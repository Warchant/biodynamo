#ifndef LOCAL_BIOLOGY_ABSTRACT_LOCAL_BIOLOGY_MODULE_H_
#define LOCAL_BIOLOGY_ABSTRACT_LOCAL_BIOLOGY_MODULE_H_

#include <memory>
#include <exception>

#include "local_biology/local_biology_module.h"

namespace cx3d {
namespace local_biology {

//fixme change to pure virtual function after porting has been finished
/**
 * Abstract class implementing the <code>LocalBiologyModule</code> interface. This class can be extended
 * to design new local modules. By default, each copy method returns <code>false</code>.
 */
class AbstractLocalBiologyModule : public LocalBiologyModule {
 public:
  AbstractLocalBiologyModule() {
  }

  virtual ~AbstractLocalBiologyModule() {
  }

  virtual StringBuilder& simStateToJson(StringBuilder& sb) const override {
    sb.append("{");
    //cellElement is circular reference
    return sb;
  }

  virtual void run() override {
    //fnoexceptionthrow std::logic_error(
        "AbstractLocalBiologyModule::run must never be called - Java must provide implementation at this point");
  }

  virtual std::shared_ptr<CellElement> getCellElement() const override {
    return cell_element_;
  }

  virtual void setCellElement(const std::shared_ptr<CellElement>& cell_element) override {
    cell_element_ = cell_element;
  }

  virtual std::shared_ptr<LocalBiologyModule> getCopy() const override {
    //fnoexceptionthrow std::logic_error(
        "AbstractLocalBiologyModule::getCopy must never be called - Java must provide implementation at this point");
  }

  virtual bool isCopiedWhenNeuriteBranches() const override {
    return false;
  }

  virtual bool isCopiedWhenSomaDivides() const override {
    return false;
  }

  virtual bool isCopiedWhenNeuriteElongates() const override {
    return false;
  }

  virtual bool isCopiedWhenNeuriteExtendsFromSoma() const override {
    return false;
  }

  virtual bool isDeletedAfterNeuriteHasBifurcated() const override {
    return false;
  }

  virtual bool equalTo(const std::shared_ptr<AbstractLocalBiologyModule>& other) {
    return this == other.get();
  }

 protected:
  std::shared_ptr<CellElement> cell_element_;

 private:
  AbstractLocalBiologyModule(const AbstractLocalBiologyModule&) = delete;
  AbstractLocalBiologyModule& operator=(const AbstractLocalBiologyModule&) = delete;
};

}  // namespace local_biology
}  // namespace cx3d

#endif  // LOCAL_BIOLOGY_ABSTRACT_LOCAL_BIOLOGY_MODULE_H_
