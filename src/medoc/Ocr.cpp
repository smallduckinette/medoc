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

#include "Ocr.h"

#include <tesseract/baseapi.h>

#include "MedocConfig.h"

Ocr::Ocr(const wxString & language):
  _api(new tesseract::TessBaseAPI)
{
  MedocConfig config;
  _api->Init(config.getTesseractDataPath().mb_str(wxConvUTF8), language.mb_str(wxConvUTF8));
}

wxString Ocr::recognize(const wxImage & image) const
{
  char * text = _api->TesseractRect(image.GetData(), 
                                    3,
                                    3 * image.GetWidth(),
                                    0,
                                    0,
                                    image.GetWidth(),
                                    image.GetHeight());
  wxString outputText(text, wxConvUTF8);
  delete[] text;
  return outputText;
}
