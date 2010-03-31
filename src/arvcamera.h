/* Aravis - Digital camera library
 *
 * Copyright © 2009-2010 Emmanuel Pacaud
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * Author: Emmanuel Pacaud <emmanuel@gnome.org>
 */

#ifndef ARV_CAMERA_H
#define ARV_CAMERA_H

#include <arvtypes.h>

G_BEGIN_DECLS

#define ARV_TYPE_CAMERA             (arv_camera_get_type ())
#define ARV_CAMERA(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), ARV_TYPE_CAMERA, ArvCamera))
#define ARV_CAMERA_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), ARV_TYPE_CAMERA, ArvCameraClass))
#define ARV_IS_CAMERA(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ARV_TYPE_CAMERA))
#define ARV_IS_CAMERA_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), ARV_TYPE_CAMERA))
#define ARV_CAMERA_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj), ARV_TYPE_CAMERA, ArvCameraClass))

typedef struct _ArvCameraPrivate ArvCameraPrivate;
typedef struct _ArvCameraClass ArvCameraClass;

struct _ArvCamera {
	GObject	object;

	ArvCameraPrivate *priv;
};

struct _ArvCameraClass {
	GObjectClass parent_class;
};

GType arv_camera_get_type (void);

ArvCamera *	arv_camera_new			(const char *name);
ArvStream *	arv_camera_get_stream		(ArvCamera *camera);
void		arv_camera_start_acquisition	(ArvCamera *camera);
void		arv_camera_stop_acquisition	(ArvCamera *camera);
guint		arv_camera_get_payload		(ArvCamera *camera);
void		arv_camera_set_region		(ArvCamera *camera, gint x, gint y, gint width, gint height);
void		arv_camera_get_region		(ArvCamera *camera, gint *x, gint *y, gint *width, gint *height);
void		arv_camera_set_binning		(ArvCamera *camera, gint dx, gint dy);
void		arv_camera_get_binning		(ArvCamera *camera, gint *dx, gint *dy);
void 		arv_camera_set_exposure_time 	(ArvCamera *camera, double exposure_time_us);
gboolean 	arv_camera_get_exposure_time 	(ArvCamera *camera);
void 		arv_camera_set_gain	 	(ArvCamera *camera, gint64 gain);
gint64 		arv_camera_get_gain 		(ArvCamera *camera);

G_END_DECLS

#endif
