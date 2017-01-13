# php7-check-taboo-word
===================

php extension for spam word filter based on Double-Array Trie tree, it can detect if a spam word exists in a text message.

关键词过滤扩展，用于检查一段文本中是否出现敏感词，基于Double-Array Trie 树实现。

# PHP版本

PHP 7

## 依赖库

[libdatrie-0.2.4 or later](http://linux.thai.net/~thep/datrie/datrie.html)

## 安装步骤

下面的$LIB_PATH为依赖库安装目录，$INSTALL_PHP_PATH为PHP5安装目录。

### 安装libdatrie
    $ tar zxvf libdatrie-0.2.4.tar.gz
    $ cd libdatrie-0.2.4
    $ make clean
    $ ./configure --prefix=$LIB_PATH
    $ make
    $ make install

### 安装扩展   
    $ $INSTALL_PHP_PATH/bin/phpize
    $ ./configure --with-php-config=$INSTALL_PHP_PATH/bin/php-config --with-trie_filter=$LIB_PATH
    $ make
    $ make install

然后修改php.ini，增加一行：extension=taboo.so，然后重启PHP。

### 包含接口

+ function taboo_init($strDictFile)
	初始化trie树，句柄放入全局变量中；之后就不会再初始化trie树
+ function taboo_search($strContent)
	查找敏感词出现的位置
+ function taboo_istaboo($strContent)
	检查是否存在敏感词

## 使用示例
	<?php
	$resTrie = taboo_init('words.dic');
	if (!$resTrie) {
	    die('create trie failed');
	}

	$str = 'hello, fuck';
	$arrRet = taboo_istaboo($str);
	print_r($arrRet); //1

	$arrRet = taboo_search($str);
	print_r($arrRet); //Array(0 => 6, 1 => 4)


Windows is not support until now.

## License

Apache License 2.0

## 致谢

本项目是在[用于检测敏感词的 PHP 扩展 php-ext-trie-filter](https://github.com/wulijun/php-ext-trie-filter)的基础上改写的。

