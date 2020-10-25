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

///
/// General Settings
///

if ('admin' != $mode) {
    require __DIR__ . '/header.php';
}

// DELETEDAYS - How many days the Event History covers. Must match the value
//              of DeleteDays in hlstats.conf.
define('DELETEDAYS', 7);

// MODE - Sets the player-tracking mode. Must match the value of Mode in
//        hlstats.conf. Possible values:
//           1) "Normal"    - Recommended for public Internet server use.
//                            Players will be tracked by Unique ID.
//           2) "NameTrack" - Useful for shared-PC environments, such as
//                            Internet cafes, etc. Players will be tracked
//                            by nickname. EXPERIMENTAL!
//           3) "LAN"       - Useful for LAN servers where players do not
//                            have a real Unique ID. Players will be tracked
//                            by IP Address. EXPERIMENTAL!
define('MODE', 'Normal');

// PLATFORM - Sets the operating system being used. Recognised values:
//               "POSIX"    - Any variant of Linux or Unix.
//               "Windows"  - Any variant of Microsoft Windows.
//            Most parts of HLstats should work on any platform, but this
//            setting allows for some OS-specific workarounds etc.
define('PLATFORM', 'Linux');

////
//// Initialisation
////

define('VERSION', '1.30 Beta');

define('SCRIPTURL', XOOPS_URL . '/modules/xhlstats/index.php');
define('ADMIN_SCRIPTURL', XOOPS_URL . '/modules/xhlstats/admin/index.php');

$modes = [
    'players',
    'clans',
    'weapons',
    'maps',
    'actions',
    'claninfo',
    'playerinfo',
    'weaponinfo',
    'mapinfo',
    'actioninfo',
    'playerhistory',
    'search',
    'admin',
    'help',
    'live_stats',
];
