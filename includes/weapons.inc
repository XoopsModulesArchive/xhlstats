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
	
	
	
	// Weapon Statistics
	
	$sql = $xoopsDB->query('SELECT name FROM ' . $xoopsDB->prefix('hlstats_Games') . " WHERE code='$game'");
	if ($xoopsDB->getRowsNum($sql) < 1) error("No such game '$game'.");
	
	[$gamename] = $xoopsDB->fetchRow($sql);
	$xoopsDB->freeRecordSet($sql);
	
	pageHeader(
        [$gamename, 'Weapon Statistics'],
        [$gamename =>"%s?game=$game", 'Weapon Statistics' => '']
	);


	$tblWeapons = new Table(
        [
            new TableColumn(
                'weapon', 'Weapon',
                'width=21&type=weaponimg&align=center&link=' . urlencode("mode=weaponinfo&amp;weapon=%k&amp;game=$game")
			),
            new TableColumn(
                'modifier', 'Points Modifier', 'width=10&align=right'
			),
            new TableColumn(
                'kills', 'Kills', 'width=12&align=right'
			),
            new TableColumn(
                'percent', 'Percentage of Kills', 'width=40&sort=no&type=bargraph'
			),
            new TableColumn(
                'percent', '%',
                'width=12&sort=no&align=right&append=' . urlencode('%')
			)
		], 'weapon', 'kills', 'weapon',
        true,
        9999, 'weap_page', 'weap_sort', 'weap_sortorder'
	);
	
	$sql1 = $xoopsDB->query(
        '
		SELECT
			COUNT(*)
		FROM
			' . $xoopsDB->prefix('hlstats_Events_Frags') . '
		LEFT JOIN ' . $xoopsDB->prefix('hlstats_Players') . ' ON
			' . $xoopsDB->prefix('hlstats_Players') . '.playerId = ' . $xoopsDB->prefix('hlstats_Events_Frags') . '.killerId
		WHERE
			' . $xoopsDB->prefix('hlstats_Players') . ".game = '$game'
	");
	
	[$totalkills] = $xoopsDB->fetchRow($sql1);

	$result = $xoopsDB->query(
        '
		SELECT
			' . $xoopsDB->prefix('hlstats_Events_Frags') . '.weapon,
			IFNULL(' . $xoopsDB->prefix('hlstats_Weapons') . '.modifier, 1.00) AS modifier,
			COUNT(' . $xoopsDB->prefix('hlstats_Events_Frags') . '.weapon) AS kills,
			COUNT(' . $xoopsDB->prefix('hlstats_Events_Frags') . ".weapon) / $totalkills * 100 AS percent
		FROM
			" . $xoopsDB->prefix('hlstats_Events_Frags') . '
		LEFT JOIN ' . $xoopsDB->prefix('hlstats_Weapons') . ' ON
			' . $xoopsDB->prefix('hlstats_Weapons') . '.code = ' . $xoopsDB->prefix('hlstats_Events_Frags') . '.weapon
		LEFT JOIN ' . $xoopsDB->prefix('hlstats_Players') . ' ON
			' . $xoopsDB->prefix('hlstats_Players') . '.playerId = ' . $xoopsDB->prefix('hlstats_Events_Frags') . '.killerId
		WHERE
			' . $xoopsDB->prefix('hlstats_Players') . ".game='$game'
			AND (" . $xoopsDB->prefix('hlstats_Weapons') . ".game='$game' OR " . $xoopsDB->prefix('hlstats_Weapons') . '.weaponId IS NULL)
		GROUP BY
			' . $xoopsDB->prefix('hlstats_Events_Frags') . ".weapon
		ORDER BY
			$tblWeapons->sort $tblWeapons->sortorder,
			$tblWeapons->sort2 $tblWeapons->sortorder
	");
?>
<table width="90%" align="center" border=0 cellspacing=0 cellpadding=0>

<tr>
	<td width="50%"><font class="fontNormal">From a total of <b><?php echo $totalkills; ?></b> kills (Last <?php echo DELETEDAYS; ?> Days)</font></td>
	<td width="50%" align="right"><font class="fontNormal">Back to <a href="<?php echo $g_options['scripturl'] . "?game=$game"; ?>"><?php echo $gamename; ?></a></font></td>
</tr>

</table><p>
<?php $tblWeapons->draw($result, $xoopsDB->getRowsNum($result), 90);
?>
