#include "application_context.hpp"

ApplicationContext::ApplicationContext(Time& time, Input& input, Application& application) noexcept
    : time{ time },
      input{ input },
      application{ application } { }
