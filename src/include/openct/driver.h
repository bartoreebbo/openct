/*
 * Core functions of the IFD handler library
 *
 * Copyright (C) 2003, Olaf Kirch <okir@suse.de>
 */

#ifndef OPENCT_DRIVER_H
#define OPENCT_DRIVER_H

#include <openct/device.h>

struct ifd_driver_ops {
	int		default_protocol;

	int		(*open)(ifd_reader_t *, const char *);
	int		(*close)(ifd_reader_t *);
	int		(*change_parity)(ifd_reader_t *, int);
	int		(*change_speed)(ifd_reader_t *, unsigned int);

	int		(*activate)(ifd_reader_t *);
	int		(*deactivate)(ifd_reader_t *);

	int		(*card_status)(ifd_reader_t *, int, int *);
	int		(*card_reset)(ifd_reader_t *, int, void *, size_t);
	int		(*card_request)(ifd_reader_t *, int,
				time_t, const char *,
				void *, size_t);
	int		(*card_eject)(ifd_reader_t *, int,
				time_t, const char *);

	int		(*output)(ifd_reader_t *, const char *);
	int		(*perform_verify)(ifd_reader_t *,
			       	int, unsigned int, const char *,
				const unsigned char *, size_t,
				unsigned char *, size_t);

	int		(*send)(ifd_reader_t *reader,
				unsigned int dad,
				const unsigned char *buffer,
				size_t len);
	int		(*recv)(ifd_reader_t *reader,
				unsigned int dad,
				unsigned char *buffer,
				size_t len,
				long timeout);

	/* These functions are used by drivers that transparently
	 * transmit all APDUs to the device, and let the device
	 * deal with T=0/T=1 etc */
	int		(*set_protocol)(ifd_reader_t *, int, int);
	int		(*transparent)(ifd_reader_t *, int,
				const void *sbuf, size_t slen,
				void *rbuf, size_t rlen);

	/* Support for synchronous ICCs */
	int		(*sync_read)(ifd_reader_t *, int, int,
				unsigned short addr,
				unsigned char *, size_t);
	int		(*sync_write)(ifd_reader_t *, int, int,
				unsigned short addr,
				const unsigned char *, size_t);
};

extern void		ifd_driver_register(const char *,
				struct ifd_driver_ops *);
extern const ifd_driver_t *ifd_driver_get(const char *);
extern void		ifd_driver_add_id(const char *, const char *);
extern const char *	ifd_driver_for_id(ifd_devid_t *);

/*
 * Protocol handling for card terminal drivers
 */
extern ifd_protocol_t *	ifd_protocol_select(ifd_reader_t *, int, int);
extern int		ifd_protocol_transceive(ifd_protocol_t *proto,
				int dad,
				const void *, size_t,
				void *, size_t);


#endif /* OPENCT_DRIVER_H */
