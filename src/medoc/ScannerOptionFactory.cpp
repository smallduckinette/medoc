// Copyright 2011 small_duck
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "ScannerOptionFactory.h"

#include <stdexcept>
#include "ScannerOptionImpl.h"

std::shared_ptr<ScannerOption> 
ScannerOptionFactory::create(wxWindow * wxWindow,
                             const SANE_Handle & handle,
                             int index,
                             const SANE_Option_Descriptor * descriptor) const
{
  assert(isValidOption(descriptor));

  if(descriptor->type == SANE_TYPE_STRING
     && descriptor->constraint_type == SANE_CONSTRAINT_STRING_LIST)
  {
    return std::shared_ptr<ScannerOption>
      (new ScannerOptionStringChoice(wxWindow,
                                     handle,
                                     index,
                                     descriptor->title,
                                     descriptor->desc,
                                     descriptor->constraint.string_list));
  }
  else if(descriptor->type == SANE_TYPE_INT || descriptor->type == SANE_TYPE_FIXED)
  {
    if(descriptor->constraint_type == SANE_CONSTRAINT_RANGE)
    {
      return std::shared_ptr<ScannerOption>
        (new ScannerOptionIntRange(wxWindow,
                                   handle,
                                   index,
                                   descriptor->title,
                                   descriptor->desc,
                                   descriptor->constraint.range));
    }
    else
    {
      return std::shared_ptr<ScannerOption>
        (new ScannerOptionInt(wxWindow,
                              handle,
                              index,
                              descriptor->title,
                              descriptor->desc));
    }
  }
  else if(descriptor->type == SANE_TYPE_BOOL)
  {
    return std::shared_ptr<ScannerOption>
      (new ScannerOptionBool(wxWindow,
                             handle,
                             index,
                             descriptor->title,
                             descriptor->desc));
  }
  else if(descriptor->type == SANE_TYPE_GROUP || descriptor->type == SANE_TYPE_BUTTON)
  {
    return std::shared_ptr<ScannerOption>
      (new ScannerOptionGroup(wxWindow,
                              handle,
                              index,
                              descriptor->title,
                              descriptor->desc));
  }
  else
  {
    throw std::runtime_error("Unexpected option");
  }
}

bool ScannerOptionFactory::isValidOption(const SANE_Option_Descriptor * descriptor) const
{
  return(descriptor->name);
}

bool ScannerOptionFactory::isGroup(const SANE_Option_Descriptor * descriptor, wxString & groupName)
{
  if(descriptor->type == SANE_TYPE_GROUP)
  {
    groupName = wxString(descriptor->title, wxConvUTF8);
    return true;
  }
  else
  {
    return false;
  }
}
