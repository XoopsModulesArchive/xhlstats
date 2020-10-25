<?php
/************************************************************************/

/* Donations - Paypal financial management module for Xoops 2           */
/* Copyright (c) 2004 by Xoops2 Donations Module Dev Team			    */
/* http://dev.xoops.org/modules/xfmod/project/?group_id=1060			*/
/*                                                                      */
/************************************************************************/
/*                                                                      */
/* Based on NukeTreasury for PHP-Nuke - by Dave Lawrence AKA Thrash     */
/* NukeTreasury - Financial management for PHP-Nuke                     */
/* Copyright (c) 2004 by Dave Lawrence AKA Thrash                       */
/*                       thrash@fragnastika.com                         */
/*                       thrashn8r@hotmail.com                          */
/*                                                                      */
/************************************************************************/
/*                                                                      */
/* This program is free software; you can redistribute it and/or modify */
/* it under the terms of the GNU General Public License as published by */
/* the Free Software Foundation; either version 2 of the License.       */
/*                                                                      */
/* This program is distributed in the hope that it will be useful, but  */
/* WITHOUT ANY WARRANTY; without even the implied warranty of           */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU     */
/* General Public License for more details.                             */
/*                                                                      */
/* You should have received a copy of the GNU General Public License    */
/* along with this program; if not, write to the Free Software          */
/* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  */
/* USA                                                                  */
/************************************************************************/

require dirname(__DIR__, 3) . '/include/cp_header.php';
require_once dirname(__DIR__) . '/settings.php';
define('ADMIN_FILES', 'true');
define('INCLUDE_PATH', '../includes');
include INCLUDE_PATH . '/functions.php';

////
//// Main
////

$mode = $_GET['mode'];

if (!in_array($mode, $modes, true)) {
    $mode = 'admin';
}

if ('admin' == $mode) {
    xoops_cp_header();

    include INCLUDE_PATH . "/$mode.inc";

    xoops_cp_footer();
} else {
    redirect_header($g_options['scripturl'] . '?mode=' . $mode, 3, 'You requested a non-admin page.');
}
