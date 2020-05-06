module Sql = {
  module Issue = {
    open Model.Issue;
    let get_all = [%rapper
      get_many(
        {sql|
        SELECT
          uuid as @string{id},
          @string{title},
          @string?{description},
          @string{board},
          @string?{assignee},
          @string{status}
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
          uuid as @string{id},
          @string{title},
          @string?{description},
          @string{board},
          @string?{assignee},
          @string{status}
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
          uuid as @string{id},
          @string{title},
          @string?{description},
          @string{board},
          @string?{assignee},
          @string{status}
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
          uuid as @string{id},
          @string{title},
          @string?{description},
          @string{board},
          @string?{assignee},
          @string{status}
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
          %string{board},
          %string?{assignee},
          %string{status}
         ) ON CONFLICT (uuid)
         DO UPDATE SET
           title = %string{title},
           description = %string?{description},
           board = %string{board},
           assignee = %string?{assignee},
           status = %string{status}
|sql},
        record_in,
      )
    ];

    let clean = [%rapper
      execute({sql|
        TRUNCATE TABLE users_users CASCADE;
        |sql})
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
          @string{owner},
          @string{status}
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
          @string{owner},
          @string{status}
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
          @string{owner},
          @string{status}
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
  };
};

module Issue = {
  let get_all = connection => Sql.Issue.get_all(connection);
  let get_all_by_board = (~id, connection) =>
    Sql.Issue.get_all_by_board(connection, ~id);
  let get_all_by_user = (~id, connection) =>
    Sql.Issue.get_all_by_user(connection, ~id);
  let upsert = (~issue, connection) => Sql.Issue.upsert(connection, issue);
};

module Board = {
  let get_all = connection => Sql.Board.get_all(connection);
  let get_all_by_user = (~id, connection) =>
    Sql.Board.get_all_by_user(connection, ~id);
  let get = (~id, connection) => Sql.Board.get(connection, ~id);
  let upsert = (board, connection) => Sql.Board.upsert(connection, board);
};

let clean = connection => {
  Sql.Issue.clean(connection, ());
};
