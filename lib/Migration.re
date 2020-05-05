let create_boards_table = [%rapper
  execute(
    {sql|
CREATE TABLE issues_boards (
  id serial,
  uuid uuid NOT NULL,
  title VARCHAR(128) NOT NULL,
  owner INTEGER NOT NULL,
  status VARCHAR(128) NOT NULL,
  created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (id),
  UNIQUE (uuid)
);
|sql},
  )
];

let create_issues_table = [%rapper
  execute(
    {sql|
CREATE TABLE issues_issues (
  id serial,
  uuid uuid NOT NULL,
  title VARCHAR(128) NOT NULL,
  description VARCHAR(512),
  board INTEGER,
  status VARCHAR(128) NOT NULL,
  created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (id),
  UNIQUE (uuid),
  FOREIGN KEY (board) REFERENCES issues_boards (id)
);
|sql},
  )
];

let migration = () => (
  "users",
  [
    ("create boards table", create_boards_table),
    ("create issues table", create_issues_table),
  ],
);
