R"(
CREATE TABLE text (
  book_id INTEGER NOT NULL,
  chapter_id INTEGER NOT NULL,
  verse_id INTEGER NOT NULL,
  word_id INTEGER NOT NULL,  -- Word ID in the verse.

  word_text TEXT NOT NULL,

  item_id INTEGER NOT NULL GENERATED ALWAYS AS (  -- Global word text ID.
                                                  -- 64 bits, int_least64_t
                        -- 1 bit is a sign of integer.
    (book_id    << 48) +  -- 15 bits, 32768  (77), int_least16_t
    (chapter_id << 32) +  -- 16 bits, 65536 (150), int_least16_t
    (verse_id   << 16) +  -- 16 bits, 65536 (176), int_least16_t
    word_id             -- 16 bits, 65536  (90), int_least16_t
  ) VIRTUAL,

  item_uuid INTEGER NOT NULL GENERATED ALWAYS AS (item_id) VIRTUAL,
--  item_uuid INTEGER NOT NULL UNIQUE,  -- For the editing purpose.

  PRIMARY KEY (book_id, chapter_id, verse_id, word_id)
) WITHOUT ROWID;
)"
