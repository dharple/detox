#!/usr/bin/env php
<?php
/**
 * Used in the unit test for src/config_file_spoof.c
 */

$file = dirname(dirname(__FILE__)) . '/etc/detoxrc';

if (!file_exists($file)) {
    printf("can't load %s\n", $file);
    exit(1);
}

$maxLength = 72;

$output = [];
$build = '';
$lines = file($file);
foreach ($lines as $line) {
    $line = trim($line);
    if (empty($line)) {
        continue;
    }
    if ($line[0] == '#') {
        continue;
    }
    $line = preg_replace('/ *{ */', '{', $line);
    $line = preg_replace('/ *}; */', '};', $line);
    $line = str_replace('"', '\"', $line);
    if (strlen($build) + strlen($line) > $maxLength) {
        $output[] = $build;
        $build = '';
    }

    $build .= $line;
}
$output[] = $build;
printf("static char *detoxrc =\n");
foreach ($output as $line) {
    printf("    \"%s\"\n", $line);
}
printf("    ;\n");
