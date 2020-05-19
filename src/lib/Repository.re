module Sql = {
  module Issue = {
    open Model.Issue;
    let get_all = [%rapper
      get_many(
        {sql|
        SELECT
          issues_issues.uuid as @string{id},
          issues_issues.title as @string{title},
          issues_issues.description as @string?{description},
          issues_boards.uuid as @string{board},
          issues_issues.assignee as @string?{assignee},
          issues_issues.status as @string{status}
        FROM issues_issues
        LEFT JOIN issues_boards
        ON issues_boards.id = issues_issues.board;
        |sql},
        record_out,
      )
    ];

    let get_all_by_board = [%rapper
      get_many(
        {sql|
        SELECT
          issues_issues.uuid as @string{id},
          issues_issues.title as @string{title},
          issues_issues.description as @string?{description},
          issues_boards.uuid as @string{board},
          issues_issues.assignee as @string?{assignee},
          issues_issues.status as @string{status}
        FROM issues_issues
        LEFT JOIN issues_boards
        ON issues_boards.id = issues_issues.board
        WHERE issues_boards.uuid = %string{id}
        |sql},
        record_out,
      )
    ];

    let get_all_by_user = [%rapper
      get_many(
        {sql|
        SELECT
          issues_issues.uuid as @string{id},
          issues_issues.title as @string{title},
          issues_issues.description as @string?{description},
          issues_boards.uuid as @string{board},
          issues_issues.assignee as @string?{assignee},
          issues_issues.status as @string{status}
        FROM issues_issues
        LEFT JOIN issues_boards
        ON issues_boards.id = issues_issues.board
        WHERE issues_issues.assignee = %string{id}
        |sql},
        record_out,
      )
    ];

    let get = [%rapper
      get_one(
        {sql|
        SELECT
          issues_issues.uuid as @string{id},
          issues_issues.title as @string{title},
          issues_issues.description as @string?{description},
          issues_boards.uuid as @string{board},
          issues_issues.assignee as @string?{assignee},
          issues_issues.status as @string{status}
        FROM issues_issues
        LEFT JOIN issues_boards
        ON issues_boards.id = issues_issues.board
        WHERE issues_issues.uuid = %string{id}
        |sql},
        record_out,
      )
    ];

    let upsert = [%rapper
      execute(
        {sql|
         INSERT INTO issues_issues (
           uuid,
           title,
           description,
           board,
           assignee,
           status
         ) VALUES (
          %string{id},
          %string{title},
          %string?{description},
          (SELECT issues_boards.id FROM issues_boards
            WHERE issues_boards.uuid = %string{board}),
          %string?{assignee},
          %string{status}
         ) ON CONFLICT (uuid)
         DO UPDATE SET
           title = %string{title},
           description = %string?{description},
           board = (SELECT issues_boards.id FROM issues_boards WHERE issues_boards.uuid = %string{board}),
           assignee = %string?{assignee},
           status = %string{status}
|sql},
        record_in,
      )
    ];

    let clean = [%rapper
      execute(
        {sql|
        TRUNCATE TABLE issues_issues CASCADE;
        |sql},
      )
    ];
  };

  module Board = {
    open Model.Board;

    let get_all = [%rapper
      get_many(
        {sql|
        SELECT
          issues_boards.uuid as @string{id},
          issues_boards.title as @string{title},
          issues_boards.owner as @string{owner},
          issues_boards.status as @string{status}
        FROM issues_boards
        |sql},
        record_out,
      )
    ];

    let get_all_by_user = [%rapper
      get_many(
        {sql|
        SELECT
          issues_boards.uuid as @string{id},
          issues_boards.title as @string{title},
          issues_boards.owner as @string{owner},
          issues_boards.status as @string{status}
        FROM issues_boards
        WHERE issues_boards.owner = %string{id}
        |sql},
        record_out,
      )
    ];

    let get = [%rapper
      get_one(
        {sql|
        SELECT
          issues_boards.uuid as @string{id},
          issues_boards.title as @string{title},
          issues_boards.owner as @string{owner},
          issues_boards.status as @string{status}
        FROM issues_boards
        WHERE issues_boards.uuid = %string{id}
        |sql},
        record_out,
      )
    ];

    let upsert = [%rapper
      execute(
        {sql|
         INSERT INTO issues_boards (
           uuid,
           title,
           owner,
           status
         ) VALUES (
           %string{id},
           %string{title},
           %string{owner},
           %string{status}
         ) ON CONFLICT (uuid)
         DO UPDATE SET
           title = %string{title},
           owner = %string{owner},
           status = %string{status}
        |sql},
        record_in,
      )
    ];

    let clean = [%rapper
      execute(
        {sql|
        TRUNCATE TABLE issues_boards CASCADE;
        |sql},
      )
    ];
  };
};

module Migration = {
  let create_boards_table = [%rapper
    execute(
      {sql|
CREATE TABLE issues_boards (
  id serial,
  uuid uuid NOT NULL,
  title VARCHAR(128) NOT NULL,
  owner uuid NOT NULL,
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
  assignee uuid,
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
    "issues",
    [
      ("create boards table", create_boards_table),
      ("create issues table", create_issues_table),
    ],
  );
};

module Issue = {
  let get_all = connection => Sql.Issue.get_all(connection, ());
  let get_all_by_board = (~id, connection) =>
    Sql.Issue.get_all_by_board(connection, ~id);
  let get_all_by_user = (~id, connection) =>
    Sql.Issue.get_all_by_user(connection, ~id);
  let upsert = (~issue, connection) => Sql.Issue.upsert(connection, issue);
  let get = (~id, connection) => Sql.Issue.get(connection, ~id);
};

module Board = {
  let get_all = connection => Sql.Board.get_all(connection, ());
  let get_all_by_user = (~user_id, connection) =>
    Sql.Board.get_all_by_user(connection, ~id=user_id);
  let get = (~id, connection) => Sql.Board.get(connection, ~id);
  let upsert = (~board, connection) => Sql.Board.upsert(connection, board);
};

let ( let* ) = Lwt_result.bind;

let clean = connection => {
  let* () = Sql.Issue.clean(connection, ());
  Sql.Board.clean(connection, ());
};

let migrate = Migration.migration;
