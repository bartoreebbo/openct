/*
 * Core functions of the IFD handler library
 *
 * Copyright (C) 2003, Olaf Kirch <okir@suse.de>
 */

#ifndef IFD_INTERNAL_H
#define IFD_INTERNAL_H

#include "core.h"
#include "config.h"
#include "device.h"
#include "driver.h"
#include "logging.h"
#include "error.h"

struct ifd_device {
	char *			name;
	int			type;
	unsigned int		etu;
	long			timeout;
	ifd_device_params_t	settings;

	struct ifd_device_ops *	ops;

	/* data follows */
};

struct ifd_device_ops {
	/* Try to identify the attached device. In the case of
	 * a serial device, perform serial PnP. For USB devices,
	 * get the vendor/device ID */
	int			(*identify)(ifd_device_t *, char *, size_t);

	/* Reset device */
//	int			(*reset)(ifd_device_t *, void *, size_t);

	int			(*set_params)(ifd_device_t *, const ifd_device_params_t *);
	int			(*get_params)(ifd_device_t *, ifd_device_params_t *);

	/* Flush any pending input */
	void			(*flush)(ifd_device_t *);

	/*
	 * Send/receive data. Some devices such as USB will support
	 * the transceive command, others such as serial devices will
	 * need to use send/recv
	 */
	int			(*transceive)(ifd_device_t *,
					ifd_apdu_t *, long);
	int			(*send)(ifd_device_t *, const void *, size_t);
	int			(*recv)(ifd_device_t *, void *, size_t, long);

	void			(*close)(ifd_device_t *);
};

struct ifd_protocol {
	int			id;
	const char *		name;
	int			(*attach)(ifd_reader_t *);
	void			(*detach)(ifd_reader_t *);
	int			(*set_param)(ifd_reader_t *, int, long);
	int			(*get_param)(ifd_reader_t *, int, long *);
	int			(*transceive)(ifd_reader_t *, unsigned char,
						ifd_apdu_t *);
};

extern ifd_protocol_t		ifd_protocol_t1;

typedef struct ifd_buf {
	unsigned char *		base;
	unsigned int		head, tail, size;
} ifd_buf_t;

/* driver.c */
extern void		ifd_driver_register(const char *,
				struct ifd_driver_ops *);
extern const ifd_driver_t *ifd_driver_get(const char *);
extern const char *	ifd_driver_for_id(const char *);

/* device.c */
extern ifd_device_t *	ifd_device_new(const char *, struct ifd_device_ops *,
				size_t);
extern void		ifd_device_free(ifd_device_t *);

/* Checksum functions */
extern unsigned int	csum_lrc_compute(const unsigned char *, size_t, unsigned char *);
extern unsigned int	csum_crc_compute(const unsigned char *, size_t, unsigned char *);

/* Buffer handling */
extern void		ifd_buf_init(ifd_buf_t *, void *, size_t);
extern void		ifd_buf_clear(ifd_buf_t *);
extern int		ifd_buf_get(ifd_buf_t *, void *, size_t);
extern int		ifd_buf_put(ifd_buf_t *, const void *, size_t);
extern unsigned int	ifd_buf_avail(ifd_buf_t *);
extern unsigned int	ifd_buf_tailroom(ifd_buf_t *);

/* module.c */
extern int		ifd_load_module(const char *, const char *);

/* utils.c */
extern void		ifd_revert_bits(unsigned char *, size_t);
extern unsigned int	ifd_count_bits(unsigned int);

#endif /* IFD_INTERNAL_H */