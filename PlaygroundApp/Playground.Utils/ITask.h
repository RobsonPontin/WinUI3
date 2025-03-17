#pragma once

namespace Playground::Utils
{
	/// <summary>
	/// The priority will push new job tasks to the top of the queue to be processed.
	/// </summary>
	enum TaskPriority
	{
		High,
		Low
	};

	struct ITask
	{
		virtual TaskPriority Priority() = 0;
		virtual bool Execute() = 0;
	};

	struct ITaskComparator
	{
		bool operator()(std::shared_ptr<ITask> lhs, std::shared_ptr<ITask> rhs) 
		{
			return lhs->Priority() > rhs->Priority(); // Min-heap (largest value has highest priority)
		}
	};
}