#include <c-efi.h>

#define NULL C_EFI_NULL

#define ERR_CHECK(s)  \
  if (C_EFI_ERROR(s)) \
    return s;

#define MAIN_PRINT(msg) \
  ERR_CHECK(Print(SystemTable, msg L"\n"));

CEfiStatus
Print(CEfiSystemTable *st, CEfiChar16 *msg)
{
  return st->con_out->output_string(st->con_out, msg);
};

CEfiStatus efi_main(CEfiHandle EfiHandle, CEfiSystemTable *SystemTable)
{
  CEfiStatus status;

  status = SystemTable->con_out->clear_screen(SystemTable->con_out);
  ERR_CHECK(status)
  MAIN_PRINT(L"BOOTX INIT");

  CEfiGraphicsOutputProtocol *gop;
  { // -- Graphics
    CEfiGuid gopGuid = C_EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;

    status = SystemTable->boot_services->locate_protocol(&gopGuid, NULL, (void **)&gop);
    if (C_EFI_ERROR(status))
    {
      MAIN_PRINT(L"LOCATE GOP FAIL");
      return status;
    }
    else
    {
      MAIN_PRINT(L"LOCATE GOP SUCCESS");
    }

    CEfiGraphicsOutputProtocolModeInformation *ModeInfo;
    CEfiUSize ModeInfoSize;

    status = gop->query_mode(gop, gop->mode == NULL ? 0 : gop->mode->mode, &ModeInfoSize, &ModeInfo);
    if (status == C_EFI_NOT_STARTED)
    {
      MAIN_PRINT(L"QUERY GOP MODE SET 0");
      status = gop->set_mode(gop, 0);
    }
    else if (C_EFI_ERROR(status))
    {
      MAIN_PRINT(L"GOP UNABLE TO GET NATIVE MODE")
      return status;
    }

    CEfiU32 nativeMode = gop->mode->mode;
    CEfiU32 numModes = gop->mode->max_mode;

    for (int i = 0; i < numModes; i++)
    {
      status = gop->query_mode(gop, i, &ModeInfoSize, &ModeInfo);
      MAIN_PRINT(L"mode %03d width %d height %d format %x%s",
                 i,
                 info->HorizontalResolution,
                 info->VerticalResolution,
                 info->PixelFormat,
                 i == nativeMode ? "(current)" : "");
    }
  }

  for (;;)
    ;
  return C_EFI_SUCCESS;
}