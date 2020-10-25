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
	 
	if ($auth->userdata['acclevel'] < 80) die ('Access denied!');
?>

&nbsp;&nbsp;&nbsp;&nbsp;<img src="<?php echo $g_options['imgdir']; ?>/downarrow.gif" width=9 height=6 border=0 align="middle" alt="downarrow.gif"><b>&nbsp;<?php
	if ($hostgroup)
	{
?><a href="<?php echo $g_options['admin_scripturl']; ?>?mode=admin&task=<?php echo $selTask; ?>"><?php
	}
	echo $task->title;
	if ($hostgroup)
	{
		echo '</a>';
	}
?></b> (Last <?php echo DELETEDAYS; ?> Days)<?php
	if ($hostgroup)
	{
?><br>
<img src="<?php echo $g_options['imgdir']; ?>/spacer.gif" width=1 height=8 border=0><br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src="<?php echo $g_options['imgdir']; ?>/downarrow.gif" width=9 height=6 border=0 align="middle" alt="downarrow.gif"><b>&nbsp;<?php echo $hostgroup; ?></b><p>
<?php
	}
	else
	{
		echo '<p>';
	}
?>

<?php
	if ($hostgroup)
	{
		$table = new Table(
            [
				new TableColumn(
                    'host', 'Host', 'width=41'
				),
				new TableColumn(
                    'freq', 'Connects', 'width=12&align=right'
				),
				new TableColumn(
                    'percent', 'Percentage of Connects', 'width=30&sort=no&type=bargraph'
				),
				new TableColumn(
                    'percent', '%',
                    'width=12&sort=no&align=right&append=' . urlencode('%')
				)
            ], 'host',			// keycol
            'freq',			// sort
            'host',			// sort2
            true,			// showranking
            50				// numperpage
		);
		
		if ($hostgroup == '(Unresolved IP Addresses)')
			$hostgroup = '';
		
		$result = $xoopsDB->query(
            '
			SELECT
				COUNT(*),
				COUNT(DISTINCT ipAddress)
			FROM
				' . $xoopsDB->prefix('hlstats_Events_Connects') . "
			WHERE
				hostgroup='$hostgroup'
		");
		
		[$totalconnects, $numitems] = $xoopsDB->fetchRow($result);
		
		$result = $xoopsDB->query("
			SELECT
				IF(hostname='', ipAddress, hostname) AS host,
				COUNT(hostname) AS freq,
				(COUNT(hostname) / $totalconnects) * 100 AS percent
			FROM
				".$xoopsDB->prefix('hlstats_Events_Connects')."
			WHERE
				hostgroup='$hostgroup'
			GROUP BY
				host
			ORDER BY
				$table->sort $table->sortorder,
				$table->sort2 $table->sortorder
			LIMIT
				$table->startitem,$table->numperpage
		");
		
		$table->draw($result, $numitems, 100, '');
	}
	else
	{
		$table = new Table(
            [
                new TableColumn(
                    'hostgroup', 'Host',
                    'width=41&icon=server&link=' . urlencode('mode=admin&task=tools_ipstats&hostgroup=%k')
				),
                new TableColumn(
                    'freq', 'Connects', 'width=12&align=right'
				),
                new TableColumn(
                    'percent', 'Percentage of Connects', 'width=30&sort=no&type=bargraph'
				),
                new TableColumn(
                    'percent', '%',
                    'width=12&sort=no&align=right&append=' . urlencode('%')
				)
			], 'hostgroup',	// keycol
            'freq',			// sort
            'hostgroup',	// sort2
            true,			// showranking
            50				// numperpage
		);
		
		$result = $xoopsDB->query(
            '
			SELECT
				COUNT(*),
				COUNT(DISTINCT hostgroup)
			FROM
				' . $xoopsDB->prefix('hlstats_Events_Connects') . '
		'
        );
		
		[$totalconnects, $numitems] = $xoopsDB->fetchRow($result);
		
		$result = $xoopsDB->query("
			SELECT
				IF(hostgroup='', '(Unresolved IP Addresses)', hostgroup) AS hostgroup,
				COUNT(hostgroup) AS freq,
				(COUNT(hostgroup) / $totalconnects) * 100 AS percent
			FROM
				".$xoopsDB->prefix('hlstats_Events_Connects')."
			GROUP BY
				hostgroup
			ORDER BY
				$table->sort $table->sortorder,
				$table->sort2 $table->sortorder
			LIMIT
				$table->startitem,$table->numperpage
		");
		
		$table->draw($result, $numitems, 100, '');
	}
?>
