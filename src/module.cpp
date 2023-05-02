#include "context.hpp"
#include "contextFactory.hpp"
#include "background.hpp"
#include "todo.hpp"
#include "withCancel.hpp"
#include "withDeadline.hpp"
#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include "pybind11/chrono.h"

using namespace context;

void addContexts(pybind11::module& m) {
    pybind11::class_<Context, std::shared_ptr<Context>>(m, "Context")
        .def("deadline", &Context::deadline)
        .def("done", &Context::done)
        .def("err", &Context::err)
        .def("cancel", &Context::cancel);

    pybind11::class_<Background, Context, std::shared_ptr<Background>>(m, "Background").def(pybind11::init<>());
    pybind11::class_<TODO, Context, std::shared_ptr<TODO>>(m, "TODO").def(pybind11::init<>());
    pybind11::class_<WithCancel, Context, std::shared_ptr<WithCancel>>(m, "WithCancel").def(pybind11::init<std::shared_ptr<Context>>());
    pybind11::class_<WithDeadline, Context, std::shared_ptr<WithDeadline>>(m, "WithDeadline")
        .def(pybind11::init<context::time, std::shared_ptr<Context>>());
}

PYBIND11_MODULE(context, m) {
    addContexts(m);
}