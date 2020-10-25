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

// Allows HLstats to work with register_globals Off
if (function_exists('ini_get')) {
    $globals = ini_get('register_globals');
} else {
    $globals = get_cfg_var('register_globals');
}
if (1 != $globals) {
    @extract($_SERVER, EXTR_SKIP);

    @extract($_COOKIE, EXTR_SKIP);

    @extract($_POST, EXTR_SKIP);

    @extract($_GET, EXTR_SKIP);

    @extract($_ENV, EXTR_SKIP);
}

// Check PHP configuration

if (version_compare(phpversion(), '4.1.0', '<')) {
    error('HLstats requires PHP version 4.1.0 or newer (you are running PHP version ' . phpversion() . ').');
}

if (!get_magic_quotes_gpc()) {
    error('HLstats requires <b>magic_quotes_gpc</b> to be <i>enabled</i>. Check your php.ini or refer to the PHP manual for more information.');
}

if (get_magic_quotes_runtime()) {
    error('HLstats requires <b>magic_quotes_runtime</b> to be <i>disabled</i>. Check your php.ini or refer to the PHP manual for more information.');
}

///
/// Classes
///

//
// Table
//
// Generates an HTML table from a DB result.
//

class Table
{
    public $columns;

    public $keycol;

    public $sort;

    public $sortorder;

    public $sort2;

    public $page;

    public $showranking;

    public $numperpage;

    public $var_page;

    public $var_sort;

    public $var_sortorder;

    public $sorthash;

    public $columnlist;

    public $startitem;

    public $maxpagenumbers = 20;

    public function __construct(
        $columns,
        $keycol,
        $sort_default,
        $sort_default2,
        $showranking = false,
        $numperpage = 50,
        $var_page = 'page',
        $var_sort = 'sort',
        $var_sortorder = 'sortorder',
        $sorthash = '',
        $sort_default_order = 'desc'
    ) {
        global $_GET;

        $this->columns = $columns;

        $this->keycol = $keycol;

        $this->showranking = $showranking;

        $this->numperpage = $numperpage;

        $this->var_page = $var_page;

        $this->var_sort = $var_sort;

        $this->var_sortorder = $var_sortorder;

        $this->sorthash = $sorthash;

        $this->sort_default_order = $sort_default_order;

        if (isset($_GET[$var_page])) {
            $this->page = (int)$_GET[$var_page];
        }

        if (isset($_GET[$var_sort])) {
            $this->sort = $_GET[$var_sort];
        }

        if (isset($_GET[$var_sortorder])) {
            $this->sortorder = $_GET[$var_sortorder];
        }

        if ($this->page < 1) {
            $this->page = 1;
        }

        $this->startitem = ($this->page - 1) * $this->numperpage;

        foreach ($columns as $col) {
            if ('no' != $col->sort) {
                $this->columnlist[] = $col->name;
            }
        }

        if (!is_array($this->columnlist) || !in_array($this->sort, $this->columnlist, true)) {
            $this->sort = $sort_default;
        }

        if ('asc' != $this->sortorder && 'desc' != $this->sortorder) {
            $this->sortorder = $this->sort_default_order;
        }

        if ($this->sort == $sort_default2) {
            $this->sort2 = $sort_default;
        } else {
            $this->sort2 = $sort_default2;
        }
    }

    public function draw($result, $numitems, $width = 100, $align = 'center')
    {
        global $g_options, $game, $xoopsDB;

        $numpages = ceil($numitems / $this->numperpage); ?>

        <table width="<?php echo $width; ?>%" align="<?php echo $align; ?>" border=0 cellspacing=0 cellpadding=0>

            <tr>
                <td>
                    <table width="100%" border=0 cellspacing=1 cellpadding=4>

                        <tr valign="bottom">
                            <?php
                            $totalwidth = 0;

        if ($this->showranking) {
            $totalwidth += 5;

            echo "<td width=\"5%\" align=\"right\"><font class=\"fontSmall\">Rank</font></td>\n";
        }

        foreach ($this->columns as $col) {
            $totalwidth += $col->width;

            echo '<td width="' . $col->width . "%\" align=\"$col->align\">";

            if ('no' != $col->sort) {
                echo getSortArrow(
                    $this->sort,
                    $this->sortorder,
                    $col->name,
                    $col->title,
                    $this->var_sort,
                    $this->var_sortorder,
                    $this->sorthash
                );
            } else {
                echo '<font class="fontSmall">';

                echo $col->title;

                echo '</font>';
            }

            echo "</td>\n";
        } ?>
                        </tr>

                        <?php
                        if (100 != $totalwidth) {
                            error("Warning: Column widths do not add to 100%! (=$totalwidth%)", false);
                        }

        $rank = ($this->page - 1) * $this->numperpage + 1;

        while (false !== ($rowdata = $xoopsDB->fetchArray($result))) {
            echo "<tr>\n";

            $i = 0;

            if ($this->showranking) {
                $c = ($i % 2) + 1;

                $i++;

                echo "<td align=\"right\"><font class=\"fontNormal\">$rank</font></td>\n";
            }

            foreach ($this->columns as $col) {
                $c = ($i % 2) + 1;

                $cellbody = '';

                $colval = $rowdata[$col->name];

                if ('left' != $col->align) {
                    $colalign = " align=\"$col->align\"";
                } else {
                    $colalign = '';
                }

                if ($col->icon) {
                    $cellbody = '&nbsp;';
                }

                if ($col->link) {
                    $link = preg_replace('%k', urlencode($rowdata[$this->keycol]), $col->link);

                    $cellbody .= '<a href="';

                    if (ADMIN_FILES == 'true') {
                        $cellbody .= $g_options['admin_scripturl'];
                    } else {
                        $cellbody .= $g_options['scripturl'];
                    }

                    $cellbody .= "?$link\">";
                }

                if ($col->icon) {
                    $cellbody .= '<img src="' . $g_options['imgdir'] . "/$col->icon.gif\" width=16 height=16 hspace=4 " . "border=0 align=\"middle\" alt=\"$col->icon.gif\">";
                }

                switch ($col->type) {
                                    case 'weaponimg':
                                        $colval = mb_strtolower(preg_replace("[ \r\n\t]*", '', $colval));

                                        $image = getImage("/weapons/$game/$colval");

                                        // check if image exists
                                        if ($image) {
                                            $cellbody .= '<img src="' . $image['url'] . '" ' . $image['size'] . ' border=0 alt="' . mb_strtoupper($colval) . '">';
                                        } else {
                                            $cellbody .= '<font class="weapon"><b>';

                                            $cellbody .= mb_strtoupper($colval);

                                            $cellbody .= '</b></font>';
                                        }

                                        break;
                                    case 'bargraph':
                                        $cellbody .= '<img src="' . $g_options['imgdir'] . '/bar';

                                        if ($colval > 40) {
                                            $cellbody .= '6';
                                        } elseif ($colval > 30) {
                                            $cellbody .= '5';
                                        } elseif ($colval > 20) {
                                            $cellbody .= '4';
                                        } elseif ($colval > 10) {
                                            $cellbody .= '3';
                                        } elseif ($colval > 5) {
                                            $cellbody .= '2';
                                        } else {
                                            $cellbody .= '1';
                                        }

                                        $cellbody .= '.gif" width="';

                                        if ($colval < 1) {
                                            $cellbody .= '1%';
                                        } elseif ($colval > 100) {
                                            $cellbody .= '100%';
                                        } else {
                                            $cellbody .= sprintf('%d%%', $colval + 0.5);
                                        }

                                        $cellbody .= "\" height=10 border=0 alt=\"$colval%\">";

                                        break;
                                    default:
                                        if ($this->showranking && 1 == $rank && 1 == $i) {
                                            $cellbody .= '<b>';
                                        }

                                        $colval = nl2br(htmlentities($colval, ENT_QUOTES | ENT_HTML5));

                                        if ('yes' == $col->embedlink) {
                                            $colval = preg_replace('%A%([^ %]+)%', '<a href="\\1">', $colval);

                                            $colval = preg_replace('%/A%', '</a>', $colval);
                                        }

                                        $cellbody .= $colval;

                                        if ($this->showranking && 1 == $rank && 1 == $i) {
                                            $cellbody .= '</b>';
                                        }

                                        break;
                                }

                if ($col->link) {
                    $cellbody .= '</a>';
                }

                /*if ($col->append)
                {
                    $cellbody .= $col->append;
                }*/

                echo "<td$colalign>" . '<font class="fontNormal">' . $cellbody . '</font>' . "</td>\n";

                $i++;
            }

            echo "</tr>\n\n";

            $rank++;
        } ?>
                    </table>
                </td>
            </tr>

        </table>
        <?php
        if ($numpages > 1) {
            ?>
            <p>
            <table width="<?php echo $width; ?>%" align="<?php echo $align; ?>" border=0 cellspacing=0 cellpadding=0>

                <tr valign="top">
                    <td width="100%" align="right"><?php
                        echo '<font class="fontNormal">';

            echo 'Page: ';

            $start = $this->page - (int)($this->maxpagenumbers / 2);

            if ($start < 1) {
                $start = 1;
            }

            $end = $numpages;

            if ($end > $this->maxpagenumbers + $start - 1) {
                $end = $this->maxpagenumbers + $start - 1;
            }

            if ($end - $start + 1 < $this->maxpagenumbers) {
                $start = $end - $this->maxpagenumbers + 1;
            }

            if ($start < 1) {
                $start = 1;
            }

            if ($start > 1) {
                if ($start > 2) {
                    $this->_echoPageNumber(1, 'First page', '', ' ...');
                } else {
                    $this->_echoPageNumber(1, 1);
                }
            }

            for ($i = $start; $i <= $end; $i++) {
                if ($i == $this->page) {
                    echo "<b>$i</b> ";
                } else {
                    $this->_echoPageNumber($i, $i);
                }

                if ($i == $end && $i < $numpages) {
                    if ($i < $numpages - 1) {
                        $this->_echoPageNumber($numpages, 'Last page', '... ');
                    } else {
                        $this->_echoPageNumber($numpages, 10);
                    }
                }
            }

            echo '</font>'; ?></td>
                </tr>

            </table><p>
            <?php
        }
    }

    public function _echoPageNumber($number, $label, $prefix = '', $postfix = '')
    {
        global $g_options;

        echo "$prefix<a href=\"" . $g_options['scripturl'] . '?' . makeQueryString($this->var_page, $number);

        if ($this->sorthash) {
            echo "#$this->sorthash";
        }

        echo "\">$label</a>$postfix ";
    }
}

//
// TableColumn
//
// Data structure for the properties of a column in a Table
//

class TableColumn
{
    public $name;

    public $title;

    public $align = 'left';

    public $width = 20;

    public $icon;

    public $link;

    public $sort = 'yes';

    public $type = 'text';

    public $embedlink = 'no';

    public function __construct($name, $title, $attrs = '')
    {
        $this->name = $name;

        $this->title = $title;

        $allowed_attrs = [
            'align',
            'width',
            'icon',
            'link',
            'sort',
            'append',
            'type',
            'embedlink',
        ];

        parse_str($attrs);

        foreach ($allowed_attrs as $a) {
            if (isset($$a)) {
                $this->$a = $$a;
            }
        }
    }
}

$g_options = getOptions();
$g_options['scripturl'] = SCRIPTURL;
$g_options['admin_scripturl'] = ADMIN_SCRIPTURL;
$g_options['imgdir'] = XOOPS_URL . '/modules/xhlstats/' . $g_options['imgdir'];

///
/// Functions
///

//
// void error (string message, [boolean exit])
//
// Formats and outputs the given error message. Optionally terminates script
// processing.
//

function error($message, $exit = true)
{
    global $g_options; ?>
    <table border=1 cellspacing=0 cellpadding=5>
        <tr>
            <td><font><b>ERROR</b></font></td>
        </tr>
        <tr>
            <td><font><?php echo $message; ?></font></td>
        </tr>
    </table>
    <?php if ($exit) {
        exit;
    }
}

//
// string makeQueryString (string key, string value, [array notkeys])
//
// Generates an HTTP GET query string from the current HTTP GET variables,
// plus the given 'key' and 'value' pair. Any current HTTP GET variables
// whose keys appear in the 'notkeys' array, or are the same as 'key', will
// be excluded from the returned query string.
//

function makeQueryString($key, $value, $notkeys = [])
{
    global $_GET;

    if (!is_array($notkeys)) {
        $notkeys = [];
    }

    $querystring = '';

    foreach ($_GET as $k => $v) {
        if ($k && $k != $key && !in_array($k, $notkeys, true)) {
            if (!eregi('PHPSESSID', $k)) {
                $querystring .= urlencode($k) . '=' . urlencode($v) . '&amp;';
            }
        }
    }

    $querystring .= urlencode($key) . '=' . urlencode($value);

    return $querystring;
}

//
// array getOptions (void)
//
// Retrieves HLstats option and style settings from the database.
//

function getOptions()
{
    global $xoopsDB;

    $result = $xoopsDB->query('SELECT keyname, value FROM ' . $xoopsDB->prefix('hlstats_Options') . '');

    $numrows = $xoopsDB->getRowsNum($result);

    if ($numrows) {
        while (false !== ($rowdata = $xoopsDB->fetchRow($result))) {
            $options[$rowdata[0]] = $rowdata[1];
        }

        return $options;
    }  

    error(
            'Warning: Could not find any options in table ' . '<b>' . $xoopsDB->prefix('hlstats_Options') . '</b>, database <b>' . DB_NAME . '</b>. Check HLstats configuration.'
        );

    return [];
}

//
// void pageHeader (array title, array location)
//
// Prints the page heading.
//

function pageHeader($title, $location)
{
    global $g_options, $_GET;

    include INCLUDE_PATH . '/header.inc';
}

//
// void pageFooter (void)
//
// Prints the page footer.
//

function pageFooter()
{
    global $g_options;

    include INCLUDE_PATH . '/footer.inc';
}

//
// void getSortArrow (string sort, string sortorder, string name,
//                    string longname, [string var_sort,
//                    string var_sortorder, string sorthash])
//
// Returns the HTML code for a sort arrow <IMG> tag.
//

function getSortArrow(
    $sort,
    $sortorder,
    $name,
    $longname,
    $var_sort = 'sort',
    $var_sortorder = 'sortorder',
    $sorthash = ''
) {
    global $g_options;

    if ('asc' == $sortorder) {
        $sortimg = 'sort-ascending.gif';

        $othersortorder = 'desc';
    } else {
        $sortimg = 'sort-descending.gif';

        $othersortorder = 'asc';
    }

    $arrowstring = '<font><a href="';

    if (ADMIN_FILES == 'true') {
        $arrowstring .= $g_options['admin_scripturl'];
    } else {
        $arrowstring .= $g_options['scripturl'];
    }

    $arrowstring .= '?' . makeQueryString($var_sort, $name, [$var_sortorder]);

    if ($sort == $name) {
        $arrowstring .= "&amp;$var_sortorder=$othersortorder";
    } else {
        $arrowstring .= "&amp;$var_sortorder=$sortorder";
    }

    if ($sorthash) {
        $arrowstring .= "#$sorthash";
    }

    $arrowstring .= '" title="Change sorting order">' . '<font>' . "$longname</font></a>";

    if ($sort == $name) {
        $arrowstring .= '&nbsp;<img src="' . $g_options['imgdir'] . "/$sortimg\"" . "width=7 height=7 hspace=4 border=0 align=\"middle\" alt=\"$sortimg\">";
    }

    $arrowstring .= '</font>';

    return $arrowstring;
}

//
// string getSelect (string name, array values, [string currentvalue])
//
// Returns the HTML for a SELECT box, generated using the 'values' array.
// Each key in the array should be a OPTION VALUE, while each value in the
// array should be a corresponding descriptive name for the OPTION.
//
// The 'currentvalue' will be given the SELECTED attribute.
//

function getSelect($name, $values, $currentvalue = '')
{
    $select = "<select name=\"$name\">\n";

    $gotcval = false;

    foreach ($values as $k => $v) {
        $select .= "\t<option value=\"$k\"";

        if ($k == $currentvalue) {
            $select .= ' selected';

            $gotcval = true;
        }

        $select .= ">$v\n";
    }

    if ($currentvalue && !$gotcval) {
        $select .= "\t<option value=\"$currentvalue\" selected>$currentvalue\n";
    }

    $select .= '</select>';

    return $select;
}

//
// string getLink (string url[, int maxlength[, string type[, string target]]])
//

function getLink($url, $maxlength = 40, $type = 'http://', $target = '_blank')
{
    if ($url && $url != $type) {
        if (preg_match("^$type(.+)", $url, $regs)) {
            $url = $type . $regs[1];
        } else {
            $url = $type . $url;
        }

        if (mb_strlen($url) > $maxlength) {
            $url_title = mb_substr($url, 0, $maxlength - 3) . '...';
        } else {
            $url_title = $url;
        }

        $url = str_replace('"', urlencode('"'), $url);

        $url = str_replace('<', urlencode('<'), $url);

        $url = str_replace('>', urlencode('>'), $url);

        return "<a href=\"$url\" target=\"$target\">" . htmlspecialchars($url_title, ENT_QUOTES | ENT_HTML5) . '</a>';
    }
  

    return '';
}

//
// string getEmailLink (string email[, int maxlength])
//

function getEmailLink($email, $maxlength = 40)
{
    if (preg_match('(.+)@(.+)', $email, $regs)) {
        if (mb_strlen($email) > $maxlength) {
            $email_title = mb_substr($email, 0, $maxlength - 3) . '...';
        } else {
            $email_title = $email;
        }

        $email = str_replace('"', urlencode('"'), $email);

        $email = str_replace('<', urlencode('<'), $email);

        $email = str_replace('>', urlencode('>'), $email);

        return "<a href=\"mailto:$email\">" . htmlspecialchars($email_title, ENT_QUOTES | ENT_HTML5) . '</a>';
    }
  

    return '';
}

//
// array getImage (string filename)
//

function getImage($filename)
{
    global $g_options;

    $url = $g_options['imgdir'] . $filename;

    if ($g_options['imgpath']) {
        $path = $g_options['imgpath'] . $filename;
    } else {
        // figure out absolute path of image

        if (!preg_match('^/', $g_options['imgdir'])) {
            preg_match('(.+)/[^/]+$', $_SERVER['SCRIPT_NAME'], $regs);

            $path = $regs[1] . '/' . $url;
        } else {
            $path = $url;
        }

        $path = $_SERVER['DOCUMENT_ROOT'] . $path;
    }

    // check if image exists

    if (file_exists($path . '.gif')) {
        $ext = 'gif';
    } elseif (file_exists($path . '.jpg')) {
        $ext = 'jpg';
    } else {
        $ext = '';
    }

    if ($ext) {
        $size = getimagesize("$path.$ext");

        return [
            'url' => "$url.$ext",
            'path' => "$path.$ext",
            'width' => $size[0],
            'height' => $size[1],
            'size' => $size[3],
        ];
    }
  

    return false;
}

?>
