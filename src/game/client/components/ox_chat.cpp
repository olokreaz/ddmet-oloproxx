//
// Created by olokreZ on 01.04.2024.
//
#include "ox_chat.h"

#include <engine/shared/config.h>
#include <engine/shared/console.h>

#include <game/client/components/chat.h>
#include <game/client/components/console.h>
#include <game/client/gameclient.h>

static void ConOxHelpLinks(IConsole::IResult *pResult, void *pUserData)
{
	IConsole *pCon = (IConsole *)pUserData;
	const auto *pBuf =
		"\n"
		"Help links :\n"
		"    ox_dummy_auto_aim_hook = safety https : // discord.com/channels/252358080522747904/293493549758939136/1222811284219625575";

	pCon->Print(IConsole::OUTPUT_LEVEL_STANDARD, "oloproxx", pBuf, {255, 0, 100, 250});
}

COxChat::COxChat()
{
}

void COxChat::OnConsoleInit()
{
	Console()->Register("/ox_help_links", "", CFGFLAG_CLIENT, ConOxHelpLinks, Console(), "Show help links");
}
