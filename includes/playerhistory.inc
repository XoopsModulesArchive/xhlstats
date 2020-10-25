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
	
	
	
	// Player History
	
	$player = (int)$_GET["player"]
		or error('No player ID specified.');
	
	$sql = $xoopsDB->query(
        '
		SELECT
			' . $xoopsDB->prefix('hlstats_Players') . '.lastName,
			' . $xoopsDB->prefix('hlstats_Players') . '.game
		FROM
			' . $xoopsDB->prefix('hlstats_Players') . "
		WHERE
			playerId=$player
	");
	if ($xoopsDB->getRowsNum($sql) != 1)
		error("No such player '$player'.");
	
	$playerdata = $xoopsDB->fetchArray($sql);
	
	
	$pl_name = $playerdata['lastName'];
	if (strlen($pl_name) > 10)
	{
		$pl_shortname = substr($pl_name, 0, 8) . '...';
	}
	else
	{
		$pl_shortname = $pl_name;
	}
	$pl_name = ereg_replace(' ', '&nbsp;', htmlspecialchars($pl_name, ENT_QUOTES | ENT_HTML5));
	$pl_shortname = ereg_replace(' ', '&nbsp;', htmlspecialchars($pl_shortname, ENT_QUOTES | ENT_HTML5));
	
	
	$game = $playerdata['game'];
	$sql2 = $xoopsDB->query('SELECT name FROM ' . $xoopsDB->prefix('hlstats_Games') . " WHERE code='$game'");
	if ($xoopsDB->getRowsNum($sql2) != 1)
		$gamename = ucfirst($game);
	else
		[$gamename] = $xoopsDB->fetchRow($sql2);
	
	
	pageHeader(
        [$gamename, 'Event History', $pl_name],
        [
            $gamename         => $g_options['scripturl'] . "?game=$game",
            'Player Rankings' => $g_options['scripturl'] . "?mode=players&amp;game=$game",
            'Player Details'  => $g_options['scripturl'] . "?mode=playerinfo&amp;player=$player",
            'Event History'   => ''
		],
        $playername
	);
	
	flush();
	
	
	$table = new Table(
        [
            new TableColumn(
                'eventTime', 'Date', 'width=20'
			),
            new TableColumn(
                'eventType', 'Type', 'width=10&align=center'
			),
            new TableColumn(
                'eventDesc', 'Description', 'width=40&sort=no&append=.&embedlink=yes'
			),
            new TableColumn(
                'serverName', 'Server', 'width=20'
			),
            new TableColumn(
                'map', 'Map', 'width=10'
			)
		], 'eventTime', 'eventTime', 'eventType',
        false,
        50, 'page', 'sort', 'sortorder'
	);
	
	$surl = $g_options['scripturl'];
	
	
	// This would be better done with a UNION query, I think, but MySQL doesn't
	// support them yet. (NOTE you need MySQL 3.23 for temporary table support.)
	
	$xoopsDB->queryF('DROP TABLE IF EXISTS ' . $xoopsDB->prefix('hlstats_EventHistory') . '');
	$xoopsDB->queryF(
        '
		CREATE TEMPORARY TABLE ' . $xoopsDB->prefix('hlstats_EventHistory') . '
		(
			eventType VARCHAR(32) NOT NULL,
			eventTime DATETIME NOT NULL,
			eventDesc VARCHAR(255) NOT NULL,
			serverName VARCHAR(32) NOT NULL,
			map VARCHAR(32) NOT NULL
		)
	'
    );
	
	function insertEvents ($table, $select)
	{
		global $xoopsDB;
		
		$select = str_replace('<table>', $xoopsDB->prefix("hlstats_Events_$table"), $select);
		$xoopsDB->queryF(
            '
			INSERT INTO
				' . $xoopsDB->prefix('hlstats_EventHistory') . "
				(
					eventType,
					eventTime,
					eventDesc,
					serverName,
					map
				)
			$select
		");
	}
	
	insertEvents(
        'TeamBonuses', "
		SELECT
			'Team Bonus',
			<table>.eventTime,
			CONCAT('My team received a points bonus of ', bonus, ' for triggering \"', " . $xoopsDB->prefix('hlstats_Actions') . ".description, '\"'),
			" . $xoopsDB->prefix('hlstats_Servers') . '.name,
			<table>.map
		FROM
			<table>
		LEFT JOIN ' . $xoopsDB->prefix('hlstats_Actions') . ' ON
			<table>.actionId = ' . $xoopsDB->prefix('hlstats_Actions') . '.id
		LEFT JOIN ' . $xoopsDB->prefix('hlstats_Servers') . ' ON
			' . $xoopsDB->prefix('hlstats_Servers') . ".serverId = <table>.serverId
		WHERE
			<table>.playerId=$player
	");
	
	if (MODE == 'LAN')
		$uqIdStr = 'IP Address:';
	else
		$uqIdStr = 'Unique ID:';
	
	insertEvents(
        'Connects', "
		SELECT
			'Connect',
			<table>.eventTime,
			CONCAT('I connected to the server'),
			" . $xoopsDB->prefix('hlstats_Servers') . '.name,
			<table>.map
		FROM
			<table>
		LEFT JOIN ' . $xoopsDB->prefix('hlstats_Servers') . ' ON
			' . $xoopsDB->prefix('hlstats_Servers') . ".serverId = <table>.serverId
		WHERE
			<table>.playerId=$player
	");
	
	insertEvents(
        'Disconnects', "
		SELECT
			'Disconnect',
			<table>.eventTime,
			'I left the game',
			" . $xoopsDB->prefix('hlstats_Servers') . '.name,
			<table>.map
		FROM
			<table>
		LEFT JOIN ' . $xoopsDB->prefix('hlstats_Servers') . ' ON
			' . $xoopsDB->prefix('hlstats_Servers') . ".serverId = <table>.serverId
		WHERE
			<table>.playerId=$player
	");
	
	insertEvents(
        'Entries', "
		SELECT
			'Entry',
			<table>.eventTime,
			'I entered the game',
			" . $xoopsDB->prefix('hlstats_Servers') . '.name,
			<table>.map
		FROM
			<table>
		LEFT JOIN ' . $xoopsDB->prefix('hlstats_Servers') . ' ON
			' . $xoopsDB->prefix('hlstats_Servers') . ".serverId = <table>.serverId
		WHERE
			<table>.playerId=$player
	");
	
	insertEvents(
        'Frags', "
		SELECT
			'Kill',
			<table>.eventTime,
			CONCAT('I killed %A%$surl?mode=playerinfo&player=', victimId, '%', " . $xoopsDB->prefix('hlstats_Players') . ".lastName, '%/A%', ' with ', weapon),
			" . $xoopsDB->prefix('hlstats_Servers') . '.name,
			<table>.map
		FROM
			<table>
		LEFT JOIN ' . $xoopsDB->prefix('hlstats_Servers') . ' ON
			' . $xoopsDB->prefix('hlstats_Servers') . '.serverId = <table>.serverId
		LEFT JOIN ' . $xoopsDB->prefix('hlstats_Players') . ' ON
			' . $xoopsDB->prefix('hlstats_Players') . ".playerId = <table>.victimId
		WHERE
			<table>.killerId=$player
	");
	
	insertEvents(
        'Frags', "
		SELECT
			'Death',
			<table>.eventTime,
			CONCAT('%A%$surl?mode=playerinfo&player=', killerId, '%', " . $xoopsDB->prefix('hlstats_Players') . ".lastName, '%/A%', ' killed me with ', weapon),
			" . $xoopsDB->prefix('hlstats_Servers') . '.name,
			<table>.map
		FROM
			<table>
		LEFT JOIN ' . $xoopsDB->prefix('hlstats_Servers') . ' ON
			' . $xoopsDB->prefix('hlstats_Servers') . '.serverId = <table>.serverId
		LEFT JOIN ' . $xoopsDB->prefix('hlstats_Players') . ' On
			' . $xoopsDB->prefix('hlstats_Players') . ".playerId = <table>.killerId
		WHERE
			<table>.victimId=$player
	");
	
	insertEvents(
        'Teamkills', "
		SELECT
			'Team Kill',
			<table>.eventTime,
			CONCAT('I killed teammate %A%$surl?mode=playerinfo&player=', victimId, '%', " . $xoopsDB->prefix('hlstats_Players') . ".lastName, '%/A%', ' with ', weapon),
			" . $xoopsDB->prefix('hlstats_Servers') . '.name,
			<table>.map
		FROM
			<table>
		LEFT JOIN ' . $xoopsDB->prefix('hlstats_Servers') . ' ON
			' . $xoopsDB->prefix('hlstats_Servers') . '.serverId = <table>.serverId
		LEFT JOIN ' . $xoopsDB->prefix('hlstats_Players') . ' On
			' . $xoopsDB->prefix('hlstats_Players') . ".playerId = <table>.victimId
		WHERE
			<table>.killerId=$player
	");
	
	insertEvents(
        'Teamkills', "
		SELECT
			'Friendly Fire',
			<table>.eventTime,
			CONCAT('My teammate %A%$surl?mode=playerinfo&player=', killerId, '%', " . $xoopsDB->prefix('hlstats_Players') . ".lastName, '%/A%', ' killed me with ', weapon),
			" . $xoopsDB->prefix('hlstats_Servers') . '.name,
			<table>.map
		FROM
			<table>
		LEFT JOIN ' . $xoopsDB->prefix('hlstats_Servers') . ' ON
			' . $xoopsDB->prefix('hlstats_Servers') . '.serverId = <table>.serverId
		LEFT JOIN ' . $xoopsDB->prefix('hlstats_Players') . ' On
			' . $xoopsDB->prefix('hlstats_Players') . ".playerId = <table>.killerId
		WHERE
			<table>.victimId=$player
	");
	
	insertEvents(
        'ChangeRole', "
		SELECT
			'Role',
			<table>.eventTime,
			CONCAT('I changed role to ', role),
			" . $xoopsDB->prefix('hlstats_Servers') . '.name,
			<table>.map
		FROM
			<table>
		LEFT JOIN ' . $xoopsDB->prefix('hlstats_Servers') . ' ON
			' . $xoopsDB->prefix('hlstats_Servers') . ".serverId = <table>.serverId
		WHERE
			<table>.playerId=$player
	");
	
	insertEvents(
        'ChangeName', "
		SELECT
			'Name',
			<table>.eventTime,
			CONCAT('I changed my name from \"', oldName, '\" to \"', newName, '\"'),
			" . $xoopsDB->prefix('hlstats_Servers') . '.name,
			<table>.map
		FROM
			<table>
		LEFT JOIN ' . $xoopsDB->prefix('hlstats_Servers') . ' ON
			' . $xoopsDB->prefix('hlstats_Servers') . ".serverId = <table>.serverId
		WHERE
			<table>.playerId=$player
	");
	
	insertEvents(
        'PlayerActions', "
		SELECT
			'Action',
			<table>.eventTime,
			CONCAT('I received a points bonus of ', bonus, ' for triggering \"', " . $xoopsDB->prefix('hlstats_Actions') . ".description, '\"'),
			" . $xoopsDB->prefix('hlstats_Servers') . '.name,
			<table>.map
		FROM
			<table>
		LEFT JOIN ' . $xoopsDB->prefix('hlstats_Servers') . ' ON
			' . $xoopsDB->prefix('hlstats_Servers') . '.serverId = <table>.serverId
		LEFT JOIN ' . $xoopsDB->prefix('hlstats_Actions') . ' ON
			' . $xoopsDB->prefix('hlstats_Actions') . ".id = <table>.actionId
		WHERE
			<table>.playerId=$player
	");
	
	insertEvents(
        'PlayerPlayerActions', "
		SELECT
			'Action',
			<table>.eventTime,
			CONCAT('I received a points bonus of ', bonus, ' for triggering \"', " . $xoopsDB->prefix('hlstats_Actions') . ".description, '\" against %A%$surl?mode=playerinfo&player=', victimId, '%', " . $xoopsDB->prefix('hlstats_Players') . ".lastName, '%/A%'),
			" . $xoopsDB->prefix('hlstats_Servers') . '.name,
			<table>.map
		FROM
			<table>
		LEFT JOIN ' . $xoopsDB->prefix('hlstats_Servers') . ' ON
			' . $xoopsDB->prefix('hlstats_Servers') . '.serverId = <table>.serverId
		LEFT JOIN ' . $xoopsDB->prefix('hlstats_Actions') . ' ON
			' . $xoopsDB->prefix('hlstats_Actions') . '.id = <table>.actionId
		LEFT JOIN ' . $xoopsDB->prefix('hlstats_Players') . ' ON
			' . $xoopsDB->prefix('hlstats_Players') . ".playerId = <table>.victimId
		WHERE
			<table>.playerId=$player
	");
	
	insertEvents(
        'PlayerPlayerActions', "
		SELECT
			'Action',
			<table>.eventTime,
			CONCAT('%A%$surl?mode=playerinfo&player=', <table>.playerId, '%', " . $xoopsDB->prefix('hlstats_Players') . ".lastName, '%/A% triggered \"', " . $xoopsDB->prefix('hlstats_Actions') . ".description, '\" against me'),
			" . $xoopsDB->prefix('hlstats_Servers') . '.name,
			<table>.map
		FROM
			<table>
		LEFT JOIN ' . $xoopsDB->prefix('hlstats_Servers') . ' ON
			' . $xoopsDB->prefix('hlstats_Servers') . '.serverId = <table>.serverId
		LEFT JOIN ' . $xoopsDB->prefix('hlstats_Actions') . ' ON
			' . $xoopsDB->prefix('hlstats_Actions') . '.id = <table>.actionId
		LEFT JOIN ' . $xoopsDB->prefix('hlstats_Players') . ' ON
			' . $xoopsDB->prefix('hlstats_Players') . ".playerId = <table>.playerId
		WHERE
			<table>.victimId=$player
	");
	
	insertEvents(
        'Suicides', "
		SELECT
			'Suicide',
			<table>.eventTime,
			CONCAT('I committed suicide with \"', weapon, '\"'),
			" . $xoopsDB->prefix('hlstats_Servers') . '.name,
			<table>.map
		FROM
			<table>
		LEFT JOIN ' . $xoopsDB->prefix('hlstats_Servers') . ' ON
			' . $xoopsDB->prefix('hlstats_Servers') . ".serverId = <table>.serverId
		WHERE
			<table>.playerId=$player
	");
	
	insertEvents(
        'ChangeTeam', "
		SELECT
			'Team',
			<table>.eventTime,
			IF(" . $xoopsDB->prefix('hlstats_Teams') . ".name IS NULL,
				CONCAT('I joined team \"', team, '\"'),
				CONCAT('I joined team \"', team, '\" (', " . $xoopsDB->prefix('hlstats_Teams') . ".name, ')')
			),
			" . $xoopsDB->prefix('hlstats_Servers') . '.name,
			<table>.map
		FROM
			<table>
		LEFT JOIN ' . $xoopsDB->prefix('hlstats_Servers') . ' ON
			' . $xoopsDB->prefix('hlstats_Servers') . '.serverId = <table>.serverId
		LEFT JOIN ' . $xoopsDB->prefix('hlstats_Teams') . ' ON
			' . $xoopsDB->prefix('hlstats_Teams') . ".code = <table>.team
		WHERE
			<table>.playerId=$player
	");
	
	
	$result = $xoopsDB->query(
        '
		SELECT
			eventTime,
			eventType,
			eventDesc,
			serverName,
			map
		FROM
			' . $xoopsDB->prefix('hlstats_EventHistory') . "
		ORDER BY
			$table->sort $table->sortorder,
			$table->sort2 $table->sortorder
		LIMIT
			$table->startitem,$table->numperpage
	");
	
	$resultCount = $xoopsDB->query(
        '
		SELECT
			COUNT(*)
		FROM
			' . $xoopsDB->prefix('hlstats_EventHistory') . '
	'
    );
	
	[$numitems] = $xoopsDB->fetchRow($resultCount);
?>
<table width="90%" align="center" border=0 cellspacing=0 cellpadding=0>

<tr>
	<td width="100%"><font class="fontNormal">&nbsp;<img src="<?php echo $g_options['imgdir']; ?>/downarrow.gif" width=9 height=6 border=0 align="middle" alt="downarrow.gif"><b>&nbsp;Player Event History</b> (Last <?php echo DELETEDAYS; ?> Days)<?php echo '</font>';?><p>

	<?php
		$table->draw($result, $numitems, 100);
	?></td>
</tr>

</table>
