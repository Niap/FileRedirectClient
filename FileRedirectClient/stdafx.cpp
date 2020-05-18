
// stdafx.cpp : 只包括标准包含文件的源文件
// FileRedirectClient.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

int InitialCommunicationPort(void)
{

	DWORD hResult = FilterConnectCommunicationPort(
		MINISPY_PORT_NAME,
		0,
		NULL,
		0,
		NULL,
		&g_hPort);

	if (hResult != S_OK) {
		return hResult;
	}
	return 0;
}

int SendMessageToPort(PVOID InputBuffer)
{
	DWORD bytesReturned = 0;
	DWORD hResult = 0;
	PCOMMAND_MESSAGE commandMessage = (PCOMMAND_MESSAGE)InputBuffer;

	hResult = FilterSendMessage(
		g_hPort,
		commandMessage,
		sizeof(COMMAND_MESSAGE),
		NULL,
		NULL,
		&bytesReturned);

	if (hResult != S_OK) {
		return hResult;
	}
	return 0;
}