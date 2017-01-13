<?php

ini_set('display_errors', 0);
ini_set('log_errors', 'on');
error_reporting(-1);
ini_set('error_log', '/var/log/php-fpm/error.log');

$st = microtime(true);
$resTrie = taboo_init('words.dic');
if (!$resTrie) {
    error_log('create trie failed');
    die('create trie failed');
}

$str = 'hello, fuck';
$arrRet = taboo_istaboo($str);
$useT = number_format(microtime(true) - $st, 6);
print_r($arrRet);
error_log("<{$useT}s> --- ".json_encode($arrRet));

/*
$resTrieA = taboo_init('words.dic');
if (!$resTrieA) {
    die('create trie failed');
}
$arrRetA = taboo_search($str);
print_r($arrRetA);
echo "\n=================\n";#
*/