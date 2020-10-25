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
	
	
	
	// Action Statistics
	
	// Addon Created by Rufus (rufus@nonstuff.de)
	
	$sql = $xoopsDB->query('SELECT name FROM ' . $xoopsDB->prefix('hlstats_Games') . " WHERE code='$game'");
	if ($xoopsDB->getRowsNum($sql) < 1) error("No such game '$game'.");
	
	[$gamename] = $xoopsDB->fetchRow($sql);
	$xoopsDB->freeRecordSet($sql);
	
	pageHeader(
        [$gamename, 'Action Statistics'],
        [$gamename =>"%s?game=$game", 'Action Statistics' => '']
	);


	$tblPlayerActions = new Table(
        [
            new TableColumn(
                'description', 'Action',
                'width=45&link=' . urlencode("mode=actioninfo&amp;action=%k&amp;game=$game")
			),
            new TableColumn(
                'obj_count', 'Achieved', 'width=25&align=right&append=+times'
			),
            new TableColumn(
                'obj_bonus', 'Skill Bonus', 'width=25&align=right'
			)
		], 'code', 'obj_count', 'description',
        true,
        9999, 'obj_page', 'obj_sort', 'obj_sortorder'
	);

	$sql2 = $xoopsDB->query(
        '
		SELECT
			COUNT(*)
		FROM
			' . $xoopsDB->prefix('hlstats_Actions') . ', ' . $xoopsDB->prefix('hlstats_Events_PlayerActions') . '
		WHERE
			' . $xoopsDB->prefix('hlstats_Events_PlayerActions') . '.actionId = ' . $xoopsDB->prefix('hlstats_Actions') . '.id
			AND ' . $xoopsDB->prefix('hlstats_Actions') . ".game='$game'
	");
	
	[$totalactions] = $xoopsDB->fetchRow($sql2);
	
	$result = $xoopsDB->query(
        '
		SELECT
			' . $xoopsDB->prefix('hlstats_Actions') . '.code,
			' . $xoopsDB->prefix('hlstats_Actions') . '.description,
			COUNT(' . $xoopsDB->prefix('hlstats_Events_PlayerActions') . '.id) AS obj_count,
			' . $xoopsDB->prefix('hlstats_Actions') . '.reward_player AS obj_bonus
		FROM
			' . $xoopsDB->prefix('hlstats_Actions') . ', ' . $xoopsDB->prefix('hlstats_Events_PlayerActions') . ', ' . $xoopsDB->prefix('hlstats_Players') . '
		WHERE
			' . $xoopsDB->prefix('hlstats_Events_PlayerActions') . '.playerId = ' . $xoopsDB->prefix('hlstats_Players') . '.playerId
			AND ' . $xoopsDB->prefix('hlstats_Players') . ".game='$game'
			AND " . $xoopsDB->prefix('hlstats_Events_PlayerActions') . '.actionId = ' . $xoopsDB->prefix('hlstats_Actions') . '.id
			AND ' . $xoopsDB->prefix('hlstats_Actions') . ".game='$game'
		GROUP BY
			" . $xoopsDB->prefix('hlstats_Actions') . ".id
		ORDER BY
			$tblPlayerActions->sort $tblPlayerActions->sortorder,
			$tblPlayerActions->sort2 $tblPlayerActions->sortorder
	");
?>
<table width="90%" align="center" border=0 cellspacing=0 cellpadding=0>

<tr>
	<td width="50%"><font class="fontNormal">From a total of <b><?php echo $totalactions; ?></b> actions (Last <?php echo DELETEDAYS; ?> Days)</font></td>
	<td width="50%" align="right"><font class="fontNormal">Back to <a href="<?php echo $g_options['scripturl'] . "?game=$game"; ?>"><?php echo $gamename; ?></a></font></td>
</tr>

</table><p>
    <?php
	$tblPlayerActions->draw($result, $xoopsDB->getRowsNum($result), 90);
?>
