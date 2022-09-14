#include "proxnlp/python/fwd.hpp"

#include "proxnlp/function-base.hpp"

namespace proxnlp::python {

struct FunctionWrap : context::Function, bp::wrapper<context::Function> {
public:
  PROXNLP_FUNCTION_TYPEDEFS(context::Scalar);

  FunctionWrap(const int nx, const int ndx, const int nr)
      : context::Function(nx, ndx, nr) {}

  ReturnType operator()(const ConstVectorRef &x) const {
    bp::override f = get_override("__call__");
    return f(x);
  }
};

struct C1FunctionWrap : context::C1Function, bp::wrapper<context::C1Function> {
  PROXNLP_FUNCTION_TYPEDEFS(context::Scalar);

  C1FunctionWrap(const int nx, const int ndx, const int nr)
      : context::C1Function(nx, ndx, nr) {}

  ReturnType operator()(const ConstVectorRef &x) const {
    bp::override f = get_override("__call__");
    return f(x);
  }

  void computeJacobian(const ConstVectorRef &x, MatrixRef Jout) const {
    Jout.resize(this->nr(), this->ndx());
    get_override("computeJacobian")(x, Jout);
  }
};

struct C2FunctionWrap : context::C2Function, bp::wrapper<context::C2Function> {
  PROXNLP_FUNCTION_TYPEDEFS(context::Scalar);

  C2FunctionWrap(const int nx, const int ndx, const int nr)
      : context::C2Function(nx, ndx, nr) {}

  ReturnType operator()(const ConstVectorRef &x) const {
    bp::override f = get_override("__call__");
    return f(x);
  }

  void computeJacobian(const ConstVectorRef &x, MatrixRef Jout) const {
    Jout.resize(this->nr(), this->ndx());
    get_override("computeJacobian")(x, Jout);
  }

  void vectorHessianProduct(const ConstVectorRef &x, const ConstVectorRef &v,
                            MatrixRef Hout) const {
    Hout.resize(this->ndx(), this->ndx());
    if (bp::override f = this->get_override("vectorHessianProduct")) {
      f(x, v, Hout);
      return;
    } else {
      return context::C2Function::vectorHessianProduct(x, v, Hout);
    }
  }

  void default_vhp(const ConstVectorRef &x, const ConstVectorRef &v,
                   MatrixRef Hout) const {
    return context::C2Function::vectorHessianProduct(x, v, Hout);
  }
};

} // namespace proxnlp::python