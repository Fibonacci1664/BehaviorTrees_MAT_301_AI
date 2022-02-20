#include <LeafNode.h>
#include <FallbackSelector.h>
#include <SequenceSelector.h>
#include <Decorator.h>
#include <Blackboard.h>
#include <iostream>
#include <conio.h>
#include <thread>
#include <chrono>

BT::NodeStatus leafAlarmFunc(BT::TreeNode* owner)
{
	std::cout << "Raising Alarm\n";

	return BT::NodeStatus::NODE_FAILURE;
}

// Layer 5
BT::NodeStatus leafStandFunction(BT::TreeNode* owner)
{
	if (BT::Blackboard::getInstance()->getAndDeleteValue("disturbance"))
	{
		return BT::NodeStatus::NODE_FAILURE;
	}

	std::cout << "Standing still..." << std::endl;

	return BT::NodeStatus::NODE_RUNNING;
}

BT::NodeStatus leafPatrolFunction(BT::TreeNode* owner)
{
	if (BT::Blackboard::getInstance()->getAndDeleteValue("disturbance"))
	{
		return BT::NodeStatus::NODE_FAILURE;
	}

	std::cout << "Patrolling..." << std::endl;

	return BT::NodeStatus::NODE_RUNNING;
}

BT::NodeStatus leafDisturbFunction(BT::TreeNode* owner)
{
	if (BT::Blackboard::getInstance()->getAndDeleteValue("disturbance"))
	{
		return BT::NodeStatus::NODE_FAILURE;
	}

	std::cout << "Moving towards disturbance..." << std::endl;

	return BT::NodeStatus::NODE_RUNNING;
}

BT::NodeStatus leafExploreFunction(BT::TreeNode* owner)
{
	if (BT::Blackboard::getInstance()->getAndDeleteValue("disturbance"))
	{
		return BT::NodeStatus::NODE_FAILURE;
	}

	std::cout << "Exploring near disturbance..." << std::endl;

	return BT::NodeStatus::NODE_RUNNING;
}

int main(int argc, char** argv)
{
	// Layer 1
	BT::FallbackSelector root("rootNode");

	// Layer 2
	BT::FallbackSelector branch1("mainBranch");
	BT::LeafNode leafFive("Alarm", leafAlarmFunc);

	root.addChildNode(&branch1);
	root.addChildNode(&leafFive);

	// Layer 3
	BT::SequenceSelector guardBranch("guard");
	BT::SequenceSelector exploreBranch("explore");

	branch1.addChildNode(&guardBranch);
	branch1.addChildNode(&exploreBranch);

	// Layer 4
	BT::Decorator leafDecor1("timeInsert", 5);		// Stand Still
	BT::Decorator leafDecor2("timeInsert", 10);		// Patrol
	BT::Decorator leafDecor3("timeInsert", 5);		// Move to Disturbance
	BT::Decorator leafDecor4("timeInsert", 5);		// Search Area

	guardBranch.addChildNode(&leafDecor1);
	guardBranch.addChildNode(&leafDecor2);

	exploreBranch.addChildNode(&leafDecor3);
	exploreBranch.addChildNode(&leafDecor4);

	// Layer 5
	BT::LeafNode leaf1("Standing", leafStandFunction);
	BT::LeafNode leaf2("Patrolling", leafPatrolFunction);
	BT::LeafNode leaf3("Disturbance", leafDisturbFunction);
	BT::LeafNode leaf4("Explore", leafExploreFunction);

	leafDecor1.addChildNode(&leaf1);
	leafDecor2.addChildNode(&leaf2);
	leafDecor3.addChildNode(&leaf3);
	leafDecor4.addChildNode(&leaf4);

	while (true)
	{
		if (_kbhit())
		{
			_getch();

			std::cout << "Guard hears a disturbance!" << std::endl;
			BT::Blackboard::getInstance()->addNewValue("disturbance", 1);
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}

		/*
			Add remaining code here
		*/

		BT::NodeStatus result = root.tick();

		if (result == BT::NodeStatus::NODE_SUCCESS)
		{
			root.reset();
		}
		else if (result == BT::NodeStatus::NODE_FAILURE)
		{
			break;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}




	return 0;
}