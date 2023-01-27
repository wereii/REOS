#include <c-efi.h>
#include <utils.h>

#define NULL C_EFI_NULL

#define ERR_CHECK(_call)  \
  {                       \
    CEfiStatus s = _call; \
    if (C_EFI_ERROR(s))   \
      return s;           \
  }

#define MAIN_PRINT(msg) \
  ERR_CHECK(Print(SystemTable, msg));

/* Print */
CEfiStatus Print(CEfiSystemTable *st, CEfiChar16 *msg)
{
  return st->con_out->output_string(st->con_out, msg);
};

typedef struct FrameBufferInfo
{
  // TODO
} t_FrameBufferInfo;

/*
GetGraphics tries to get the available graphic modes and returns VideoMemo
Returns NULL on error
*/
t_FrameBufferInfo *GetGraphics(CEfiSystemTable *st)
{
  if (st == NULL)
    return NULL;
  CEfiStatus status;
  CEfiGraphicsOutputProtocol *gop;

  CEfiGuid gopGuid = C_EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
  status = st->boot_services->locate_protocol(&gopGuid, NULL, (void **)&gop);
  if (C_EFI_ERROR(status))
  {
    Print(st, L"LOCATE GOP FAIL - SKIP");
    return NULL;
  }

  Print(st, L"LOCATE GOP SUCCESS");

  CEfiGraphicsOutputProtocolModeInformation *ModeInfo;
  CEfiUSize ModeInfoSize;

  CEfiU32 nativeMode = gop->mode->mode;
  CEfiU32 numModes = gop->mode->max_mode;

  // CEfiChar16 wchar_buff[64] = {0};

  for (unsigned int i = 0; i < numModes; i++)
  {
    status = gop->query_mode(gop, i, &ModeInfoSize, &ModeInfo);
    Print(st, L"Mode");
  }

  // TODO

  return NULL;
}

CEfiStatus efi_main(CEfiHandle EfiHandle, CEfiSystemTable *SystemTable)
{
  CEfiStatus status;

  status = SystemTable->con_out->clear_screen(SystemTable->con_out);
  ERR_CHECK(status)
  MAIN_PRINT(L"BOOTX INIT\n\r");

  /* -- Graphics -- */

  char msg[] = "Hi there\n\r!!!\n\r";
  unsigned short wchar_buff[32] = {0};

  atow(msg, sizeof(msg), wchar_buff);
  status = Print(SystemTable, wchar_buff);
  if (C_EFI_WARN_UNKNOWN_GLYPH == status)
  {
    ERR_CHECK(Print(SystemTable, L"\n\rErr unkwn glyph\n\r"));
  }
  else if (C_EFI_ERROR(status))
  {
    ERR_CHECK(Print(SystemTable, L"\n\rERR!\n\r"));
  }

  char chbuff[17] = {0};
  itohexa(0xDEADBEEF, chbuff);
  atow(chbuff, 17, wchar_buff);
  status = Print(SystemTable, wchar_buff);
  if (C_EFI_WARN_UNKNOWN_GLYPH == status)
  {
    ERR_CHECK(Print(SystemTable, L"\n\rErr unkwn glyph\n\r"));
  }
  else if (C_EFI_ERROR(status))
  {
    ERR_CHECK(Print(SystemTable, L"\n\rERR!\n\r"));
  }

  ERR_CHECK(Print(SystemTable, L"\n\rPass\n\r"));
  for (;;)
    ;
  return C_EFI_SUCCESS;
}