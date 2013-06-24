int audio_set_volume(struct audio_info_struct *ai, USHORT setvolume);
int audio_pause(struct audio_info_struct *ai, int pause);
int audio_open(struct audio_info_struct *ai);
int audio_nobuffermode(struct audio_info_struct *ai, int setnobuffermode);
int audio_trash_buffers(struct audio_info_struct *ai);
int audio_close(struct audio_info_struct *ai);
int audio_get_formats(struct audio_info_struct *ai);
int audio_get_devices(char *info, int deviceid);
void start_keyboard_thread(struct frame *fr);
