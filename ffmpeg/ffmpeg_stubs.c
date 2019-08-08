#if _MSC_VER
// some stubs to overcome MSVC dead code elimination issue for Debug build
int ff_get_cpu_flags_aarch64() { return  0; }
int ff_get_cpu_flags_arm() { return  0; }
int ff_get_cpu_flags_ppc() { return  0; }
int ff_get_cpu_flags_x86() { return  0; }
int ff_get_cpu_max_align_aarch64() { return 8; }
int ff_get_cpu_max_align_arm() { return 8; }
int ff_get_cpu_max_align_ppc() { return 8; }
int ff_get_cpu_max_align_x86() { return 8; }
int ff_frame_thread_encoder_init(void* dummy1, void* dummy2) { return -1; }
void ff_frame_thread_encoder_free(void* dummy1) {}
#endif