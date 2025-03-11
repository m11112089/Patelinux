#define MQ_ID0		'b'
#define MQ_ID1		'r'

#define	CTRL_TO_IMG		100
#define	IMG_TO_CTRL		101
#define	CTRL_TO_DIPATCH	200
#define	DIPATCH_TO_CTRL	201

#define MSG_FILE "/etc/passwd"
#define STORAGE_LOG_FILE "/run/log/storage"
#define COMPRESS_LOG_FILE "/run/log/compress"

typedef struct mdata_struct
{
	unsigned short	cmd;
	union {
		struct {
			unsigned char b_H265;		// [0] 1 = enable 0 = disable (default)
			unsigned char b_cloud;		// [1] 1 = enable 0 = disable (default)
			unsigned char b_ship;		// [2] 1 = enable 0 = disable (default)
			unsigned char b_split_img;	// [3] 1 = enable 0 = disable (default)
			unsigned char mode;			// [6-4] AIRS Mode 1~5
			unsigned char b_awb;		// [7] 1 = enable 0 = disable (default)
						
			unsigned long img_y_size;	// [15:0]
			unsigned long img_band;		// [23:16]
				                        // [31:24] RSVD
		} config;
		struct {
			unsigned long img_addr;
		} store;
		char dummy[12];
	};	
} m_data_t;

struct msg_form {
	long m_type;
	m_data_t m_data;
};

struct cmd_t {
	int cmd_id;
	int argc;
	char *argv[4];
};

extern int mq_get(char id);

extern int log_count(int count);
