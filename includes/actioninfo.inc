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
	
	// Action Details

	// Addon created by Rufus (rufus@nonstuff.de)
	
	$action = $_GET['action']
		or error('No action ID specified.');
	
	$sql = $xoopsDB->query(
        '
		SELECT
			description
		FROM
			' . $xoopsDB->prefix('hlstats_Actions') . "
		WHERE
			code='$action'
			AND game='$game'
	");
	
	if ($xoopsDB->getRowsNum($sql) != 1)
	{
		$act_name = ucfirst($action);
	}
	else
	{
		$actiondata = $xoopsDB->fetchArray($sql);
		$xoopsDB->freeRecordSet($sql);
		$act_name = $actiondata['description'];
	}
	
	$sql2 = $xoopsDB->query('SELECT name FROM ' . $xoopsDB->prefix('hlstats_Games') . " WHERE code='$game'");
	if ($xoopsDB->getRowsNum($sql2) != 1)
		error('Invalid or no game specified.');
	else
		[$gamename] = $xoopsDB->fetchRow($sql2);
		
	pageHeader(
        [$gamename, 'Action Details', $act_name],
        [
            $gamename           => $g_options['scripturl'] . "?game=$game",
            'Action Statistics' => $g_options['scripturl'] . "?mode=actions&amp;game=$game",
            'Action Details'    => ''
		],
        $act_name
	);



	$table = new Table(
        [
            new TableColumn(
                'playerName', 'Player',
                'width=45&align=left&icon=player&link=' . urlencode('mode=playerinfo&amp;player=%k')
			),
            new TableColumn(
                'obj_count', 'Achieved', 'width=25&align=right'
			),
            new TableColumn(
                'obj_bonus', 'Skill Bonus Total', 'width=25&align=right&sort=no'
			)
		], 'playerId', 'obj_count', 'playerName',
        true,
        50
	);
	
	$result = $xoopsDB->query(
        '
		SELECT
			' . $xoopsDB->prefix('hlstats_Events_PlayerActions') . '.playerId,
			' . $xoopsDB->prefix('hlstats_Players') . '.lastName AS playerName,
			COUNT(' . $xoopsDB->prefix('hlstats_Events_PlayerActions') . '.id) AS obj_count,
			COUNT(' . $xoopsDB->prefix('hlstats_Events_PlayerActions') . '.id) * ' . $xoopsDB->prefix('hlstats_Actions') . '.reward_player AS obj_bonus
		FROM
			' . $xoopsDB->prefix('hlstats_Events_PlayerActions') . ', ' . $xoopsDB->prefix('hlstats_Players') . ', ' . $xoopsDB->prefix('hlstats_Actions') . '
		WHERE
			' . $xoopsDB->prefix('hlstats_Actions') . ".code = '$action' AND
			" . $xoopsDB->prefix('hlstats_Players') . ".game = '$game' AND
			" . $xoopsDB->prefix('hlstats_Players') . '.playerId = ' . $xoopsDB->prefix('hlstats_Events_PlayerActions') . '.playerId AND
			' . $xoopsDB->prefix('hlstats_Events_PlayerActions') . '.actionId = ' . $xoopsDB->prefix('hlstats_Actions') . '.id AND
			' . $xoopsDB->prefix('hlstats_Players') . ".hideranking<>'1'
		GROUP BY
			" . $xoopsDB->prefix('hlstats_Events_PlayerActions') . ".playerId
		ORDER BY
			$table->sort $table->sortorder,
			$table->sort2 $table->sortorder
		LIMIT $table->startitem,$table->numperpage
	");
	
	$resultCount = $xoopsDB->query(
        '
		SELECT
			COUNT(DISTINCT ' . $xoopsDB->prefix('hlstats_Events_PlayerActions') . '.playerId),
			COUNT(' . $xoopsDB->prefix('hlstats_Events_PlayerActions') . '.Id)
		FROM
			' . $xoopsDB->prefix('hlstats_Events_PlayerActions') . ', ' . $xoopsDB->prefix('hlstats_Players') . ', ' . $xoopsDB->prefix('hlstats_Actions') . '
		WHERE
			' . $xoopsDB->prefix('hlstats_Actions') . ".code = '$action' AND
			" . $xoopsDB->prefix('hlstats_Players') . ".game = '$game' AND
			" . $xoopsDB->prefix('hlstats_Players') . '.playerId = ' . $xoopsDB->prefix('hlstats_Events_PlayerActions') . '.playerId AND
			' . $xoopsDB->prefix('hlstats_Events_PlayerActions') . '.actionId = ' . $xoopsDB->prefix('hlstats_Actions') . '.id
	'
    );
	
	[$numitems, $totalact] = $xoopsDB->fetchRow($resultCount);
?>
<table width="90%" align="center" border=0 cellspacing=0 cellpadding=0>

<tr>
	<td width="50%"><font class="fontNormal">From a total of <b><?php echo (int)$totalact; ?></b> achievements (Last <?php echo DELETEDAYS; ?> Days)</font></td>
	<td width="50%" align="right"><font class="fontNormal">Back to <a href="<?php echo $g_options['scripturl'] . "?mode=actions&amp;game=$game"; ?>">Action Statistics</a></font></td>
</tr>

</table><p>
    <?php
	$table->draw($result, $numitems, 70, 'center');
?>
