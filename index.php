<?php
/*
 * HLstats - Real-time player and clan rankings and statistics for Half-Life
 * http://sourceforge.net/projects/hlstats/
 *
 * Copyright (C) 2001  Simon Garner
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

require_once __DIR__ . '/settings.php';
define('ADMIN_FILES', 'false');
define('INCLUDE_PATH', 'includes');
include INCLUDE_PATH . '/functions.php';

////
//// Main
////

$mode = $_GET['mode'];

if (!in_array($mode, $modes, true)) {
    $mode = 'contents';
}

if ('admin' != $mode) {
    require XOOPS_ROOT_PATH . '/header.php';

    include INCLUDE_PATH . "/$mode.inc";

    pageFooter();

    require __DIR__ . '/footer.php';
} else {
    redirect_header('admin/index.php?mode=' . $mode, 3, 'You are being redirected to the Administration page.');
}
