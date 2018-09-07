#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
typedef int ssize_t;
#include <vlc/vlc.h>

// library version 2.2.8
char capture_name[255];
char record_option[255];
char record_name[255];
const char *url = "rtsp://192.168.10.192:8554/test";
const char *sout  = ":sout=#std{access=file,mus=mp4,dst='D:/";
libvlc_media_stats_t stats;

void set_record_option(libvlc_media_t *m_r, int number);

int main(int argc, char* argv[])
{
	libvlc_instance_t * inst;
	libvlc_media_player_t *mp;
	libvlc_media_player_t *mp_record;
	libvlc_media_t *m;
	libvlc_media_t *m_record;
	libvlc_state_t status;
	char input;
	int capture_number = 0;
	int record_number = 0;

	printf("------ COMMAND\r\n");
	printf("r: stop and save recording\r\n");
	printf("n: start recording\r\n");
	printf("c: capture\r\n");
	printf("q: quit\r\n");
	printf("--------------\r\n");
	/* Load the VLC engine */
	inst = libvlc_new(0, NULL);
//	inst = libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);

	/* Create a new item */
//	m = libvlc_media_new_location(inst, "https://www.freedesktop.org/software/gstreamer-sdk/data/media/sintel_trailer-480p.webm");
//	m = libvlc_media_new_location(inst, "rtsp://192.168.10.192:8554/test");
//	m = libvlc_media_new_path (inst, "/path/to/test.mov");
	m = libvlc_media_new_path(inst, "D:\\Projects\\Byton\\test.sdp");
	m_record = libvlc_media_new_path(inst, "D:\\Projects\\Byton\\test.sdp");

	/* Create a media player playing environement */
	mp = libvlc_media_player_new_from_media(m);
	mp_record = libvlc_media_player_new_from_media(m_record);
	set_record_option(m_record, record_number);
	record_number++;

#if 0
	/* This is a non working code that show how to hooks into a window,
	* if we have a window around */
	libvlc_media_player_set_xwindow(mp, xid);
	/* or on windows */
	libvlc_media_player_set_hwnd(mp, hwnd);
	/* or on mac os */
	libvlc_media_player_set_nsobject(mp, view);
#endif

	/* play the media_player */
	libvlc_media_player_play(mp);
	libvlc_media_player_play(mp_record);
	printf("test1\r\n");

	while (1)
	{
		input = getchar();
		if (input == 's')
		{
			if (libvlc_media_get_stats(m, &stats))
			{
				status = libvlc_media_get_state(m);
				printf("status: %d\r\n", status);
				printf("readbytes: %d\r\n", stats.i_read_bytes);
				printf("demux_read_bytes: %d\r\n", stats.i_demux_read_bytes);
				printf("demux_corrupted: %d\r\n", stats.i_demux_corrupted);
				printf("demux_discontinuity: %d\r\n", stats.i_demux_discontinuity);
				printf("decoded_video: %d\r\n", stats.i_decoded_video);
				printf("displayed_pictures: %d\r\n", stats.i_displayed_pictures);
				printf("lost_pictures: %d\r\n", stats.i_lost_pictures);
				printf("played_abuffers: %d\r\n", stats.i_played_abuffers);
				printf("lost_abuffers: %d\r\n", stats.i_lost_abuffers);
				printf("sent_packets: %d\r\n", stats.i_sent_packets);
				printf("sent_bytes: %d\r\n", stats.i_sent_bytes);
			}
			else
			{
				printf("can't load satatistics\r\n");
			}
		}
		else if (input == 'c')
		{
			sprintf_s(capture_name, "test%d.bmp", capture_number);
			if (libvlc_video_take_snapshot(mp, 0, capture_name, 640, 480) == 0)
			{
				printf("capture1 success %d\r\n", capture_number);
				capture_number++;
			}
			else
			{
				printf("capture1 fail %d\r\n", capture_number);
			}
		}
		else if (input == 'r')
		{
			if (libvlc_media_get_state(m_record) == 3)
			{
				/* Stop playing */
				libvlc_media_player_stop(mp_record);
				printf("stop recording\r\n");
			}
			else
			{
				printf("not recording\r\n");
			}
		}
		else if (input == 'n')
		{
			if (libvlc_media_get_state(m_record) != 3)
			{
				set_record_option(m_record, record_number);

				/* play the media_player */
				libvlc_media_player_play(mp_record);
				printf("start recording %d\r\n", record_number);
				record_number++;
			}
			else
			{
				printf("now recording\r\n");
			}
		}
		else if (input == 'q')
		{
			break;
		}
	}
	
	printf("stop\r\n");
	/* No need to keep the media now */
	libvlc_media_release(m);
	libvlc_media_release(m_record);
	/* Stop playing */
	libvlc_media_player_stop(mp);
	libvlc_media_player_stop(mp_record);

	/* Free the media_player */
	libvlc_media_player_release(mp);
	libvlc_media_player_release(mp_record);

	libvlc_release(inst);


	return 0;
}

void set_record_option(libvlc_media_t *m_r, int number)
{
	strcpy_s(record_option, 255, sout);
	sprintf_s(record_name, "test%d.mp4", number);
	strcpy_s(record_option + strlen(record_option), 255, record_name);
	strcpy_s(record_option + strlen(record_option), 255, "'}");
	libvlc_media_add_option(m_r, ":sout-all");
	libvlc_media_add_option(m_r, record_option);
	libvlc_media_add_option(m_r, ":sout-keep");
}
