R"(
SELECT
  book_id, chapter_id, verse_id, word_id, word_text, item_uuid
FROM text
WHERE
book_id >= ? AND chapter_id >= ? AND verse_id >= ? AND word_id >= ? AND
book_id <= ? AND chapter_id <= ? AND verse_id <= ? AND word_id <= ?
ORDER BY book_id ASC, chapter_id ASC, verse_id ASC, word_id ASC;
)"
