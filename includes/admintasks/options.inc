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
	 
	if ($auth->userdata['acclevel'] < 100) die ('Access denied!');

class OptionGroup
{
public $title = '';
public $options = [];

public function __construct($title)
{
    $this->title = $title;
}

public function draw ()
{
global $g_options;
?>
<b><?php echo $this->title; ?></b><br>
<table width="75%" border=0 cellspacing=0 cellpadding=0>

    <tr valign="top">
        <td>
            <table width="100%" border=0 cellspacing=1 cellpadding=4>
                <?php
                foreach ($this->options as $opt) {
                    $opt->draw();
                }
                ?>
            </table>
        </td>
    </tr>

</table>
<p>
    <?php
    }

    public function update()
    {
        global $_POST, $xoopsDB;

        foreach ($this->options as $opt) {
            $optval = $_POST[$opt->name];

            $result = $xoopsDB->query(
                '
					SELECT
						value
					FROM
						' . $xoopsDB->prefix('hlstats_Options') . "
					WHERE
						keyname='$opt->name'
				"
            );

            if ($xoopsDB->getRowsNum($result) == 1) {
                $result = $xoopsDB->query(
                    '
						UPDATE
							' . $xoopsDB->prefix('hlstats_Options') . "
						SET
							value='$optval'
						WHERE
							keyname='$opt->name'
					"
                );
            } else {
                $result = $xoopsDB->query(
                    '
						INSERT INTO
							' . $xoopsDB->prefix('hlstats_Options') . "
							(
								keyname,
								value
							)
						VALUES
						(
							'$opt->name',
							'$optval'
						)
					"
                );
            }
        }
    }
    }

    class Option
{
    public $name;
    public $title;
    public $type;

    public function __construct($name, $title, $type)
    {
        $this->name  = $name;
        $this->title = $title;
        $this->type  = $type;
    }

    public function draw()
    {
        global $g_options, $optiondata;

        ?>
        <tr valign="middle">
            <td width="45%"><?php
                echo '<font class="fontNormal">';
                echo $this->title . ':';
                echo '</font>';
                ?></td>
            <td width="55%"><?php
                switch ($this->type) {
                    case 'textarea':
                        echo "<textarea name=\"$this->name\" cols=35 rows=4 wrap=\"virtual\">";
                        echo htmlspecialchars($optiondata[$this->name], ENT_QUOTES | ENT_HTML5);
                        echo '</textarea>';
                        break;

                    default:
                        echo "<input type=\"text\" name=\"$this->name\" size=35 value=\"";
                        echo htmlspecialchars($optiondata[$this->name], ENT_QUOTES | ENT_HTML5);
                        echo '" class="textbox" maxlength=255>';
                }
                ?></td>
        </tr>
        <?php
    }
}
	
	$optiongroups = [];
	
	$optiongroups[1] = new OptionGroup('General');
	$optiongroups[1]->options[] = new Option('sitename', 'Site Name', 'text');
	$optiongroups[1]->options[] = new Option('siteurl', 'Site URL', 'text');
	$optiongroups[1]->options[] = new Option('contact', 'Contact URL', 'text');
	
	$optiongroups[2] = new OptionGroup('Paths');
	$optiongroups[2]->options[] = new Option('imgdir', 'Image Directory URL', 'text');
	$optiongroups[2]->options[] = new Option('imgpath', 'Image Directory Filesystem Path', 'text');
	$optiongroups[2]->options[] = new Option('map_dlurl', 'Map Download URL<br><font>(%MAP% = map, %GAME% = gamecode)</font>', 'text');
	
	if ($_POST)
	{
		foreach ($optiongroups as $og)
		{
			$og->update();
		}
		message('success', 'Options updated successfully.');
	}
	
	$result = $xoopsDB->query('SELECT keyname, value FROM ' . $xoopsDB->prefix('hlstats_Options') . '');
	while (false !== ($rowdata = $xoopsDB->fetchRow($result)))
	{
		$optiondata[$rowdata[0]] = $rowdata[1];
	}
	
	foreach ($optiongroups as $og)
	{
		$og->draw();
	}
?>

<table width="75%" border=0 cellspacing=0 cellpadding=0>
<tr>
	<td align="center"><input type="submit" value="  Apply  " class="submit"></td>
</tr>
</table>

