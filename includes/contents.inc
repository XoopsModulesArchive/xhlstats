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
	
	
	// Contents
	
	$resultGames = $xoopsDB->query(
        '
		SELECT
			code,
			name
		FROM
			' . $xoopsDB->prefix('hlstats_Games') . "
		WHERE
			hidden='0'
		ORDER BY
			name ASC
	");
	
	$num_games = $xoopsDB->getRowsNum($resultGames);
	
	if ($num_games == 1 || $game = $_GET['game'])
	{
		if ($num_games == 1)
			[$game] = $xoopsDB->fetchRow($resultGames);
		
		include INCLUDE_PATH . '/game.inc';
    } else
	{
		pageHeader(['HLStats Main'], ['HLStats Main' => '']);
        ?>

<table width="90%" align="center" border=0 cellspacing=0 cellpadding=0>

<tr>
	<td><font class="fontNormal">&nbsp;<img src="<?php echo $g_options['imgdir']; ?>/downarrow.gif" width=9 height=6 border=0 align="middle" alt="downarrow.gif"><b>&nbsp;Games</b><?php echo '</font>'; ?><p>

		<table width="80%" align="center" border=0 cellspacing=0 cellpadding=0>
		
		<tr>
			<td><table width="100%" border=0 cellspacing=1 cellpadding=4>
			
				<tr valign="bottom">
					<td width="60%" align="left"><?php echo '<font class="fontSmall">'; ?>&nbsp;Game<?php echo '</font>'; ?></td>
					<td width="20%" align="center"><?php echo '<font class="fontSmall">'; ?>&nbsp;Top Player<?php echo '</font>'; ?></td>
					<td width="20%" align="center"><?php echo '<font class="fontSmall">'; ?>&nbsp;Top Clan<?php echo '</font>'; ?></td>
				</tr>
				
<?php
				while (false !== ($gamedata = $xoopsDB->fetchRow($resultGames)))
				{
					$result = $xoopsDB->query('
						SELECT
							playerId,
							lastName
						FROM
							' . $xoopsDB->prefix('hlstats_Players') . "
						WHERE
							game='$gamedata[0]'
							AND hideranking=0
						ORDER BY
							skill DESC
						LIMIT 1
					");
		
					if ($xoopsDB->getRowsNum($result) == 1)
					{
						$topplayer = $xoopsDB->fetchRow($result);
					}
					else
					{
						$topplayer = false;
					}
					
					$result = $xoopsDB->query('
						SELECT
							' . $xoopsDB->prefix('hlstats_Clans') . '.clanId,
							' . $xoopsDB->prefix('hlstats_Clans') . '.name,
							AVG(' . $xoopsDB->prefix('hlstats_Players') . '.skill) AS skill,
							COUNT(' . $xoopsDB->prefix('hlstats_Players') . '.playerId) AS numplayers
						FROM
							' . $xoopsDB->prefix('hlstats_Clans') . '
						LEFT JOIN ' . $xoopsDB->prefix('hlstats_Players') . ' ON
							' . $xoopsDB->prefix('hlstats_Players') . '.clan = ' . $xoopsDB->prefix('hlstats_Clans') . '.clanId
						WHERE
							' . $xoopsDB->prefix('hlstats_Clans') . ".game='$gamedata[0]'
						GROUP BY
							".$xoopsDB->prefix('hlstats_Clans').'.clanId
						HAVING
							skill IS NOT NULL
							AND numplayers > 1
						ORDER BY
							skill DESC
						LIMIT 1
					'
                    );

                    if ($xoopsDB->getRowsNum($result) == 1)
					{
						$topclan = $xoopsDB->fetchRow($result);
					}
					else
					{
						$topclan = false;
					}
		?>
				
				<tr valign="middle">
					<td>
						<table width="100%" border=0 cellspacing=0 cellpadding=0>
						
						<tr valign="middle">
							<td width="100%"><font class="fontNormal"><b>&nbsp;<a href="<?php echo $g_options['scripturl'] . "?game=$gamedata[0]"; ?>"><img src="<?php
	$imgfile = $g_options['imgdir'] . "/game-$gamedata[0].gif";
	if (file_exists($imgfile))
	{
		echo $imgfile;
	}
	else
	{
		echo $g_options['imgdir'] . '/game.gif';
    }
                                            ?>" width=16 height=16 hspace=3 border=0 align="middle" alt="Game"><?php echo $gamedata[1]; ?></a></b></font></td>
							<td><?php echo '<font class="fontSmall">'; ?>&nbsp;<a href="<?php echo $g_options['scripturl'] . "?mode=players&amp;game=$gamedata[0]"; ?>"><img src="<?php echo $g_options['imgdir']; ?>/player.gif" width=16 height=16 hspace=3 alt="Player Rankings" border=0 align="middle">Players</a>&nbsp;&nbsp;<?php echo '</font>'; ?>
                            </td>
							<td><?php echo '<font class="fontSmall">'; ?>&nbsp;<a href="<?php echo $g_options['scripturl'] . "?mode=clans&amp;game=$gamedata[0]"; ?>"><img src="<?php echo $g_options['imgdir']; ?>/clan.gif" width=16 height=16 hspace=3 alt="Clan Rankings" border=0 align="middle">Clans</a>&nbsp;&nbsp;<?php echo '</font>'; ?>
                            </td>
						</tr>
						
						</table></td>
					<td align="center"><?php
						echo '<font class="fontNormal">';
                        if ($topplayer)
						{
							echo '<a href="' . $g_options['scripturl'] . '?mode=playerinfo&amp;player=' . $topplayer[0] . "\">$topplayer[1]</a>";
						}
						else
						{
							echo '-';
                        }
						echo '</font>';
                        ?></td>
					<td align="center"><?php
						echo '<font class="fontNormal">';
                        if ($topclan)
						{
							echo '<a href="' . $g_options['scripturl'] . '?mode=claninfo&amp;clan=' . $topclan[0] . "\">$topclan[1]</a>";
						}
						else
						{
							echo '-';
                        }
						echo '</font>';
                        ?></td>
				</tr>
<?php
				}
?>	
				</table></td>
		</tr>
		
		</table><p>
		<br>
		
		
		<font class="fontNormal">&nbsp;<img src="<?php echo $g_options['imgdir']; ?>/downarrow.gif" width=9 height=6 border=0 align="middle" alt="downarrow.gif"><b>&nbsp;General Statistics</b><?php echo '</font>'; ?><p>

		<?php
			$result = $xoopsDB->query('SELECT COUNT(*) FROM ' . $xoopsDB->prefix('hlstats_Players') . '');
        [$num_players] = $xoopsDB->fetchRow($result);
			$num_players = (int)$num_players;
			
			$result = $xoopsDB->query('SELECT COUNT(*) FROM ' . $xoopsDB->prefix('hlstats_Clans') . '');
        [$num_clans] = $xoopsDB->fetchRow($result);
			$num_clans = (int)$num_clans;
			
			$result = $xoopsDB->query('SELECT COUNT(*) FROM ' . $xoopsDB->prefix('hlstats_Servers') . '');
			[$num_servers] = $xoopsDB->fetchRow($result);
			$num_servers = (int)$num_servers;
			
			$result = $xoopsDB->query("
				SELECT
					DATE_FORMAT(MAX(eventTime), '%r, %a. %e %b.')
				FROM
					".$xoopsDB->prefix('hlstats_Events_Frags').'
			');
			[$lastevent] = $xoopsDB->fetchRow($result);
?>

		<table width="80%" align="center" border=0 cellspacing=0 cellpadding=3>
		
		<tr valign="top">
			<td width=10><font class="fontNormal"><b>&#149;&nbsp;</b></font></td>
			<td width="100%"><?php
				echo '<font class="fontNormal">';

                echo "<b>$num_players</b> players and <b>$num_clans</b> clans "
					. "ranked in <b>$num_games</b> games on <b>$num_servers</b>"
					. ' servers.';

                echo '</font>';
			?></td>
        </tr>

            <?php
			if ($lastevent)
			{
?>
		<tr valign="top">
			<td width=10><font class="fontNormal"><b>&#149;&nbsp;</b></font></td>
			<td width="100%"><?php
				echo '<font class="fontNormal">';

                echo "Last kill <b>$lastevent</b>";
				
				echo '</font>';
			?></td>
        </tr>
                <?php
			}
?>
		
		<tr valign="top">
			<td width=10><font class="fontNormal"><b>&#149;&nbsp;</b></font></td>
			<td width="100%"><?php
				echo '<font class="fontNormal">';

                echo 'All statistics are generated in real-time. Event history data expires after <b>' . DELETEDAYS . '</b> days.';

                echo '</font>';
			?></td>
        </tr>

        </table></td>
</tr>

</table>

<?php
	}
?>
