//
// Created by olokreZ on 01.04.2024.
//
#ifndef GAME_CLIENT_COMPONENT_OX_CHAT_H
#define GAME_CLIENT_COMPONENT_OX_CHAT_H
#include <engine/rust.h>

#include <game/client/component.h>

class COxChat : public CComponent
{
public:
	COxChat();
	int Sizeof() const override { return sizeof(*this); }
	void OnConsoleInit() override;
};

#endif // GAME_CLIENT_COMPONENT_OX_CHAT_H
