#include "taichi/program/async_utils.h"

#include "taichi/ir/analysis.h"
#include "taichi/ir/ir.h"
#include "taichi/ir/statements.h"
#include "async_utils.h"

TLANG_NAMESPACE_BEGIN

std::unique_ptr<IRNode> IRHandle::clone() const {
  // TODO: remove get_kernel() here
  return irpass::analysis::clone(const_cast<IRNode *>(ir_), ir_->get_kernel());
}

TaskLaunchRecord::TaskLaunchRecord() : kernel(nullptr), ir_handle(nullptr, 0) {
}

TaskLaunchRecord::TaskLaunchRecord(Context context,
                                   Kernel *kernel,
                                   IRHandle ir_handle)
    : context(context), kernel(kernel), ir_handle(ir_handle) {
  TI_ASSERT(ir_handle.ir()->get_kernel() != nullptr);
}

OffloadedStmt *TaskLaunchRecord::stmt() const {
  TI_ASSERT(ir_handle.ir());
  return const_cast<IRNode *>(ir_handle.ir())->as<OffloadedStmt>();
}

bool TaskLaunchRecord::empty() const {
  return ir_handle.ir() == nullptr;
}

void TaskMeta::print() const {
  fmt::print("TaskMeta\n  name {}\n", name);
  fmt::print("  type {}\n", OffloadedStmt::task_type_name(type));
  if (snode != nullptr) {
    fmt::print("  snode {}\n", snode->get_node_type_name_hinted());
  } else {
    fmt::print("  snode nullptr\n");
  }
  if (!input_states.empty()) {
    fmt::print("  input states:\n    ");
    for (auto s : input_states) {
      fmt::print("{} ", s.name());
    }
    fmt::print("\n");
  }
  if (!output_states.empty()) {
    fmt::print("  output states:\n    ");
    for (auto s : output_states) {
      fmt::print("{} ", s.name());
    }
    fmt::print("\n");
  }
}

TLANG_NAMESPACE_END
