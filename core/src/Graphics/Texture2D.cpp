﻿#include "Texture2D.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "../Common/Resources.h"
#include "../IO/File.h"
#include "Graphics.h"

namespace Altseed {
std::map<std::u16string, std::shared_ptr<std::mutex>> Texture2D::mtxs;

Texture2D::Texture2D(
        std::shared_ptr<Resources>& resources,
        std::shared_ptr<LLGI::Texture>& texture,
        uint8_t* data,
        int32_t width,
        int32_t height,
        const std::u16string& sourcePath) 
{
    sourcePath_ = sourcePath;
    size_ = Vector2DI(width, height);
    for (int32_t i = 0; i < size_.X * size_.Y; i++) {
        m_buffer.push_back(data[i]);
    }
    m_resources = resources;
    m_texture = texture;
}

Texture2D::~Texture2D() {
    m_resources->GetResourceContainer(ResourceType::Texture2D)->Unregister(sourcePath_);
    m_resources = nullptr;
    m_texture = nullptr;
}

bool Texture2D::Reload() { return false; }

std::shared_ptr<Texture2D> Texture2D::Load(const char16_t* path) {
    if (mtxs.count(path) == 0) mtxs[path] = std::make_shared<std::mutex>();
    std::lock_guard<std::mutex> lock(*mtxs[path]);

    auto resources = Resources::GetInstance();
    auto cache = std::dynamic_pointer_cast<Texture2D>(resources->GetResourceContainer(ResourceType::Texture2D)->Get(path));
    if (cache != nullptr) {
        cache->AddRef();
        return cache;
    }

    auto file = File::GetInstance()->CreateStaticFile(path);
    if (file == nullptr) {
        return nullptr;
    }

    int32_t w, h, channel;
    uint8_t* data = (uint8_t*)stbi_load_from_memory((stbi_uc*)file->GetData(), file->GetSize(), &w, &h, &channel, 0);

    if (data == nullptr) {
        return nullptr;
    }

    auto llgiTexture = Graphics::GetInstance()->CreateTexture(data, w, h, channel);
    if (llgiTexture == nullptr) {
        delete[] data;
        return nullptr;
    }

    auto res = MakeAsdShared<Texture2D>(resources, llgiTexture, data, w, h, path);
    resources->GetResourceContainer(ResourceType::Texture2D)
            ->Register(path, std::make_shared<ResourceContainer::ResourceInfomation>(res, path));

    delete[] data;

    return res;
}
}  // namespace Altseed