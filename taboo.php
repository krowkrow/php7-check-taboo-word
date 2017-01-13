<?php

/**
 * Load a trie tree from a saved trie tree file
 * 
 * @param string $strDictFile Path to saved trie tree file
 * @return true, or false if error;
 */
function taboo_load($strDictFile) {
	
}

/**
 * Find if a spam word exists in the content
 * 
 * @param string $strContent
 * @return array spam word info, like Array(0 => start position, 1 => spam word len), if no one found, return empty array, false on error.
 * @example
 * <pre>
 * $arrSpamWord = taboo_search($strContent);
 * if (! empty($arrSpamWord)) {
 *     echo substr($strContent, $arrSpamWord[0], $arrSpamWord[1]);
 * }
 * </pre>
 */
function taboo_search($strContent) {
	
}

/**
 * check if a taboo word exists in the content
 * 
 * @param string $strContent
 * @return false, or ture if has taboo word
 * @example
 * <pre>
 * $arrSpamWord = taboo_istaboo($strContent);
 * if (! empty($arrSpamWord)) {
 *     echo substr($strContent, $arrSpamWord[0], $arrSpamWord[1]);
 * }
 * </pre>
 */
function taboo_istaboo($strContent) {
	
}
