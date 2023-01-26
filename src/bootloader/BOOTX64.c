#include <c-efi.h>

#define NULL C_EFI_NULL

#define ERR_CHECK(s)  \
  if (C_EFI_ERROR(s)) \
    return s;


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
  status = Print(SystemTable, L"BOOTX INIT\n");
  ERR_CHECK(status)

  return 0;
}