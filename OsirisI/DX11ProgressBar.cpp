#include "stdafx.h"

using namespace OsirisI;
using namespace OsirisI::Graphics;
using namespace OsirisI::Graphics::Actors;
using namespace OsirisI::Graphics::Components;
using namespace OsirisI::Graphics::Renderer;
using namespace OsirisI::Graphics::Shader;
using namespace OsirisI::Manager;
using namespace OsirisI::Resources;
using namespace OsirisI::Utilities;


DX11ProgressBar::DX11ProgressBar(OsirisI::OVector2 size, OsirisI::OVector3 pos, OsirisI::Utilities::HashedString name) : IProgressBar(name) {
    this->size = size;
    this->position = pos;
    this->maxValue = 100;
    this->currentValue = 0;

    this->progressBarActive = new DX11Sprite(pos, OVector2(0.f,size.y), name.GetString() + "_SPRITE_ACTIVE", OVector4(0.f, 255.f, 0.f, 1)); //IST GRUEN -> BESTAETIGT!
    this->progressBarInactive = new DX11Sprite(pos, size, name.GetString() + "_SPRITE_INACTIVE", OVector4(255.f, 0.0f, 0.f, 1));
    
    OS_CHECKSTATE_THROW(this->progressBarActive->Initialize());
    OS_CHECKSTATE_THROW(this->progressBarInactive->Initialize());
}

DX11ProgressBar::DX11ProgressBar(OsirisI::OVector2 size, OsirisI::OVector3 pos, OsirisI::Utilities::HashedString name, OsirisI::Utilities::HashedString containerPath) : IProgressBar(name) {
    this->size = size;
    this->position = pos;
    this->name = name;
    this->maxValue = 100;
    this->currentValue = 0;
        
    this->progressBarActive = new DX11Sprite(pos, OVector2(0.f,size.y), name.GetString() + "_SPRITE_ACTIVE", containerPath);
    this->progressBarInactive = new DX11Sprite(pos, size, name.GetString() + "_SPRITE_INACTIVE", containerPath);
    
    OS_CHECKSTATE_THROW(this->progressBarActive->Initialize());
    OS_CHECKSTATE_THROW(this->progressBarInactive->Initialize());
}

ReturnState DX11ProgressBar::Initialize() {
    this->renderInfo = this->progressBarInactive->GetRenderInfo();
    this->renderInfo->nestedRenderInfo = this->progressBarActive->GetRenderInfo();
    
    return OS_OK;
}

ReturnState DX11ProgressBar::Update(float delta) {
    float currentProcess = this->currentValue / this->maxValue;
    this->progressBarInactive->SetWidth(static_cast<unsigned int>((1-currentProcess) * this->size.x));
	this->progressBarInactive->SetPositionX(static_cast<unsigned int>(this->position.x + (currentProcess * this->size.x)));

	this->progressBarActive->SetWidth(static_cast<unsigned int>(currentProcess * this->size.x));
    
    OS_CHECKSTATE(this->progressBarInactive->Update(delta));
    OS_CHECKSTATE(this->progressBarActive->Update(delta));

    return OS_OK;
}

ReturnState DX11ProgressBar::Recover() {
    return OS_OK;
}

ReturnState DX11ProgressBar::Release() {

    return OS_OK;
}

OsirisI::Graphics::Renderer::RenderInfo* DX11ProgressBar::GetRenderInfo() {       
    return this->renderInfo;
}