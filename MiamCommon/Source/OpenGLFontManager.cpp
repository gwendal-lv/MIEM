/*
  ==============================================================================

    OpenGLFontManager.cpp
    Created: 9 Dec 2018 10:36:32pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include "OpenGLFontManager.h"

using namespace Miam;

OpenGLFontManager::OpenGLFontManager()
{
    fontTextureImage = LoadImage("newFontImg_png");
}

void OpenGLFontManager::InitGLFontResources()
{
    fontTexture = std::make_unique<OpenGLTexture>();
    fontTexture->loadImage(fontTextureImage);
}
void OpenGLFontManager::ReleaseGLFontResources()
{
    fontTexture->release();
    fontTexture = nullptr;
}

Image OpenGLFontManager::LoadImage(String path)
{
    Image returnImage;
    bool isBinary = false;
    int resourceId = 0;
    for (int i = 0; i < BinaryData::namedResourceListSize; ++i)
    {
        if (path == BinaryData::namedResourceList[i])
        {
            isBinary = true;
            resourceId = i;
        }
    }
    if (isBinary)
    {
        int dataSize = 0;
        const void * srcData = BinaryData::getNamedResource(BinaryData::namedResourceList[resourceId],
                                                            dataSize);
        returnImage = resizeImageToPowerOfTwo(ImageCache::getFromMemory(srcData,dataSize));
    }
    else
        returnImage = resizeImageToPowerOfTwo(ImageFileFormat::loadFrom(File(path)));
    return returnImage;
}

Image OpenGLFontManager::resizeImageToPowerOfTwo(Image m_image)
{
    if (!(isPowerOfTwo(m_image.getWidth()) && isPowerOfTwo(m_image.getHeight())))
        return m_image.rescaled(jmin(1024, nextPowerOfTwo(m_image.getWidth())),
                                jmin(1024, nextPowerOfTwo(m_image.getHeight())));
    return m_image;
}
