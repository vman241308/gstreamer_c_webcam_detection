#!/usr/bin/env python

# Aravis - Digital camera library
#
# Copyright (c) 2011 Emmanuel Pacaud
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General
# Public License along with this library; if not, write to the
# Free Software Foundation, Inc., 59 Temple Place, Suite 330,
# Boston, MA 02111-1307, USA.
#
# Author: Emmanuel Pacaud <emmanuel@gnome.org>

#  If you have installed aravis in a non standard location, you may need
#   to make GI_TYPELIB_PATH point to the correct location. For example:
#
#   export GI_TYPELIB_PATH=$GI_TYPELIB_PATH:/opt/bin/lib/girepositry-1.0/
#
#  You may also have to give the path to libaravis.so, using LD_PRELOAD or
#  LD_LIBRARY_PATH.

import sys
import time

from gi.repository import Aravis

buffer_a = Aravis.Buffer.new (1024, None)
buffer_b = Aravis.Buffer.new (1024, None)

print "Buffer a refcount :        %d" %(buffer_a.__grefcount__)
print "Buffer a is preallocated : %d" %(buffer_a.is_preallocated)
print "Buffer b refcount :        %d" %(buffer_b.__grefcount__)
print "Buffer b is preallocated : %d" %(buffer_b.is_preallocated)

