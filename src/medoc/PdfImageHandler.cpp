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

#include "PdfImageHandler.h"

#include <sstream>
#include <Magick++.h> 
#include <wx/mstream.h>

namespace
{
  template <class Container>
  void readImages(Container * sequence,
                  const Magick::Blob & blob,
                  int dpi)
  {
    MagickCore::ImageInfo * imageInfo = MagickCore::CloneImageInfo(0);
    std::ostringstream str;
    str << dpi << "x" << dpi;
    imageInfo->density = strdup(str.str().c_str());
    MagickCore::ExceptionInfo exceptionInfo;
    MagickCore::GetExceptionInfo( &exceptionInfo );
    MagickCore::Image *images = MagickCore::BlobToImage( imageInfo,
                                                         blob.data(),
                                                         blob.length(), &exceptionInfo );
    MagickCore::DestroyImageInfo(imageInfo);
    Magick::insertImages( sequence, images );
    Magick::throwException( exceptionInfo );
    MagickCore::DestroyExceptionInfo( &exceptionInfo );
  }

  std::list<Magick::Image> loadImages(wxInputStream & stream, int dpi)
  {
    wxFileOffset offset = stream.TellI();
    wxMemoryOutputStream buffer;
    stream.Read(buffer);
    stream.SeekI(offset);
    
    Magick::Blob blob  
      (buffer.GetOutputStreamBuffer()->GetBufferStart(),
       buffer.GetOutputStreamBuffer()->GetBufferSize());
    
    std::list<Magick::Image> images;
    ::readImages(&images, blob, dpi);
    return images;
  }
}

PdfImageHandler::PdfImageHandler():
  m_dpi(150)
{
  SetName(_("PdfImageHanlder"));
  SetExtension(_("pdf"));
}

int PdfImageHandler::GetImageCount(wxInputStream & stream)
{
  return loadImages(stream, m_dpi).size();
}

bool PdfImageHandler::LoadFile(wxImage * image, wxInputStream & stream, bool verbose, int index)
{
  std::list<Magick::Image> images = loadImages(stream, m_dpi);
  std::list<Magick::Image>::iterator it = images.begin();
  if(index != -1)
  {
    std::advance(it, index);
  }
  if(it != images.end())
  {
    Magick::Image & page = *it;
    Magick::Blob blob;
    page.write(&blob);
    wxMemoryInputStream input(blob.data(), blob.length());
    *image = wxImage(input);
    return true;
  }
  else
  {
    return false;
  }
}

bool PdfImageHandler::SaveFile(wxImage * image, wxOutputStream & stream, bool verbose)
{
  return false;
}

void PdfImageHandler::setDpi(int dpi)
{
  m_dpi = dpi;
}

bool PdfImageHandler::DoCanRead(wxInputStream & stream)
{
  unsigned char hdr[4];
  if(!stream.Read(hdr, WXSIZEOF(hdr)))
  {
    return false;
  }
  else
  {
    return memcmp(hdr, "%PDF", WXSIZEOF(hdr)) == 0;
  }
}
