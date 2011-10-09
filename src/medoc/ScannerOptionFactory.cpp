#include "ScannerOptionFactory.h"

#include <stdexcept>
#include "ScannerOptionImpl.h"

std::shared_ptr<ScannerOption> 
ScannerOptionFactory::create(wxWindow * wxWindow,
                             const SANE_Option_Descriptor * descriptor) const
{
  assert(isValidOption(descriptor));

  switch(descriptor->type)
  {
  case SANE_TYPE_STRING:
    if(descriptor->constraint_type == SANE_CONSTRAINT_STRING_LIST)
    {
      return std::shared_ptr<ScannerOption>
        (new ScannerOptionStringChoice(wxWindow,
                                       descriptor->name,
                                       descriptor->title,
                                       descriptor->desc,
                                       descriptor->constraint.string_list));
    }
  case SANE_TYPE_INT:
    if(descriptor->constraint_type == SANE_CONSTRAINT_RANGE)
    {
      return std::shared_ptr<ScannerOption>
        (new ScannerOptionIntRange(wxWindow,
                                   descriptor->name,
                                   descriptor->title,
                                   descriptor->desc,
                                   descriptor->constraint.range));
    }
  default:
    break;
  }
  
  throw std::runtime_error("Unsupported option");
}

bool ScannerOptionFactory::isValidOption(const SANE_Option_Descriptor * descriptor) const
{
  return(descriptor->name);
}
