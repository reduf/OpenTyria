DROP TABLE IF EXISTS users;
CREATE TABLE users (
    user_id    VARCHAR(36),
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    account_id VARCHAR(36) NOT NULL,

    PRIMARY KEY (user_id)
);

DROP TABLE IF EXISTS sessions;
CREATE TABLE sessions (
    user_id    VARCHAR(36),
    session_id VARCHAR(36),
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    account_id VARCHAR(36) NOT NULL,

    PRIMARY KEY (user_id, session_id)
);

DROP TABLE IF EXISTS accounts;
CREATE TABLE accounts (
    account_id              VARCHAR(36),
    created_at              DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at              DATETIME DEFAULT CURRENT_TIMESTAMP,
    eula_accepted           BOOLEAN DEFAULT FALSE,
    current_char_id         VARCHAR(36),
    current_territory       INTEGER DEFAULT 0,
    storage_gold            INTEGER DEFAULT 0,
    balthazar_points_max    INTEGER DEFAULT 10000,
    balthazar_points_amount INTEGER DEFAULT 0,
    balthazar_points_total  INTEGER DEFAULT 0,
    kurzick_points_max      INTEGER DEFAULT 10000,
    kurzick_points_amount   INTEGER DEFAULT 0,
    kurzick_points_total    INTEGER DEFAULT 0,
    luxon_points_max        INTEGER DEFAULT 10000,
    luxon_points_amount     INTEGER DEFAULT 0,
    luxon_points_total      INTEGER DEFAULT 0,
    imperial_points_max     INTEGER DEFAULT 10000,
    imperial_points_amount  INTEGER DEFAULT 0,
    imperial_points_total   INTEGER DEFAULT 0,

    PRIMARY KEY (account_id)
);

DROP TABLE IF EXISTS characters;
CREATE TABLE characters (
    char_id              VARCHAR(36),
    created_at           DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at           DATETIME DEFAULT CURRENT_TIMESTAMP,
    account_id           VARCHAR(36) NOT NULL,
    char_name            BLOB(32) NOT NULL,
    skill_points         INTEGER DEFAULT 0,
    skill_points_total   INTEGER DEFAULT 0,
    experience           INTEGER DEFAULT 0,
    gold                 INTEGER DEFAULT 0,
    last_outpost         INTEGER DEFAULT 0,
    primary_profession   INTEGER NOT NULL,
    secondary_profession INTEGER DEFAULT 0,
    level                INTEGER DEFAULT 1,
    active_weapon_set    INTEGER DEFAULT 0,
    campaign             INTEGER NOT NULL,
    face                 INTEGER NOT NULL,
    hair_color           INTEGER NOT NULL,
    hair_style           INTEGER NOT NULL,
    height               INTEGER NOT NULL,
    sex                  INTEGER NOT NULL,
    skin                 INTEGER NOT NULL,
    current_item_slot    INTEGER DEFAULT 0,
    unlocked_skills      BLOB(128) DEFAULT '',
    unlocked_professions INTEGER DEFAULT 0,

    PRIMARY KEY (char_id)
);

DROP TABLE IF EXISTS friendships;
CREATE TABLE friendships (
    account_id        VARCHAR(36),
    friend_account_id VARCHAR(36),
    created_at        DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at        DATETIME DEFAULT CURRENT_TIMESTAMP,
    type              INTEGER NOT NULL,
    renamed           BLOB(32),

    PRIMARY KEY (account_id, friend_account_id)
);

DROP TABLE IF EXISTS pvp_items;
CREATE TABLE pvp_items (
    account_id        VARCHAR(36),

    PRIMARY KEY (account_id)
);

DROP TABLE IF EXISTS items;
CREATE TABLE items (
    char_id     VARCHAR(36),
    bag_enum    INTEGER NOT NULL,
    slot        INTEGER NOT NULL,
    quantity    INTEGER NOT NULL,
    dye_color   INTEGER NOT NULL,
    model       INTEGER NOT NULL,
    file_id     INTEGER NOT NULL,
    flags       INTEGER NOT NULL,

    PRIMARY KEY (char_id, bag_enum, slot)
);

DROP TABLE IF EXISTS bags;
CREATE TABLE bags (
    char_id     VARCHAR(36),
    bag_enum    INTEGER NOT NULL,
    bag_type    INTEGER NOT NULL,
    max_slots   INTEGER NOT NULL,

    PRIMARY KEY (char_id, bag_enum)
);

INSERT INTO sessions (user_id, session_id, account_id) VALUES
 ("fa520ee2-4419-4eb4-ae49-6e9abe6ef24f", "d8b9bf5d-90b1-4cbd-9b76-88da7be763b6", "58ec1cf3-2059-424b-8443-768244e401e9"),
 ("5fb222bc-9d19-4308-ac3e-3c62685bc6ae", "224aee30-d8a5-4f18-ad8e-b5fcbf121c4e", "a65d1882-f4f5-4067-b6b6-8c99671bb6b6"),
 ("9957c79c-b93c-4b60-9d84-9f38a1f62be3", "ccdb4b8d-1c88-48c0-bda0-0a5e8d87f4da", "864094a4-e0e4-40bd-a89d-051ad7c063aa");

INSERT INTO accounts (account_id, eula_accepted, current_char_id) VALUES
 ("58ec1cf3-2059-424b-8443-768244e401e9", TRUE, "c4379679-f760-4b53-9372-2e037ded8b08"),
 ("a65d1882-f4f5-4067-b6b6-8c99671bb6b6", TRUE, "209147c4-2492-471e-9e9e-09da1cbbee95"),
 ("864094a4-e0e4-40bd-a89d-051ad7c063aa", FALSE, NULL);

INSERT INTO characters (char_id, account_id, char_name, primary_profession, campaign, face, hair_color, hair_style, height, sex, skin) VALUES
 ("8eb3db49-6c09-4830-938e-03a0e3e585e7", "58ec1cf3-2059-424b-8443-768244e401e9", "Christian Herakles", 1, 0, 1, 1, 1, 3, 1, 1),
 ("0f8b69e2-6592-43c5-a61e-72a45f5e891c", "58ec1cf3-2059-424b-8443-768244e401e9", "Nayeli Pontius", 4, 1, 1, 1, 1, 3, 1, 1),
 ("18fd5be4-5465-41c4-bab9-81240dc6c8bd", "58ec1cf3-2059-424b-8443-768244e401e9", "Kerman Martino", 2, 2, 1, 1, 1, 3, 1, 1),
 ("565ad034-28c4-4c4a-a1de-20208defc9cd", "58ec1cf3-2059-424b-8443-768244e401e9", "Detlef Kresten", 8, 2, 1, 1, 1, 3, 1, 1),
 ("d8d45b60-7cad-4f3e-bfdf-faafec9b6a77", "a65d1882-f4f5-4067-b6b6-8c99671bb6b6", "Tomaso Beata", 3, 1, 1, 1, 1, 3, 1, 1),
 ("a3c9f0d4-c13d-4c9f-94ee-2165babe1a3a", "a65d1882-f4f5-4067-b6b6-8c99671bb6b6", "Tyrique Eua", 5, 1, 1, 1, 1, 3, 1, 1);
