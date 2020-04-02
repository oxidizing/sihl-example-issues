module Async = Sihl.Core.Async;

module Issue = {
  module GetAll = {
    let stmt = "
SELECT
  uuid_of(issues_issues.uuid) as id,
  issues_issues.title as title,
  issues_issues.description as description,
  uuid_of(issues_boards.uuid) as board,
  uuid_of(users_users.uuid) as assignee,
  issues_issues.status as status
FROM issues_issues
LEFT JOIN users_users
ON users_users.id = issues_issues.assignee
LEFT JOIN issues_boards
ON issues_boards.id = issues_issues.board;
";

    let query = connection =>
      Sihl.App.Repo.getMany(
        ~connection,
        ~stmt,
        ~decode=Model.Issue.t_decode,
        (),
      );
  };

  module GetAllByBoard = {
    let stmt = "
SELECT
  uuid_of(issues_issues.uuid) as id,
  issues_issues.title as title,
  issues_issues.description as description,
  uuid_of(issues_boards.uuid) as board,
  uuid_of(users_users.uuid) as assignee,
  issues_issues.status as status
FROM issues_issues
LEFT JOIN users_users
ON users_users.id = issues_issues.assignee
LEFT JOIN issues_boards
ON issues_boards.id = issues_issues.board
WHERE issues_boards.uuid = UNHEX(REPLACE(?, '-', ''));
";

    [@decco]
    type params = string;

    let query = (connection, ~boardId) =>
      Sihl.App.Repo.getMany(
        ~connection,
        ~stmt,
        ~decode=Model.Issue.t_decode,
        ~parameters=params_encode(boardId),
        (),
      );
  };

  module GetAllByUser = {
    let stmt = "
SELECT
  uuid_of(issues_issues.uuid) as id,
  issues_issues.title as title,
  issues_issues.description as description,
  uuid_of(issues_boards.uuid) as board,
  uuid_of(users_users.uuid) as assignee,
  issues_issues.status as status
FROM issues_issues
LEFT JOIN users_users
ON users_users.id = issues_issues.assignee
LEFT JOIN issues_boards
ON issues_boards.id = issues_issues.board
WHERE users_users.uuid = UNHEX(REPLACE(?, '-', ''));
";

    [@decco]
    type params = string;

    let query = (connection, ~userId) =>
      Sihl.App.Repo.getMany(
        ~connection,
        ~stmt,
        ~decode=Model.Issue.t_decode,
        ~parameters=params_encode(userId),
        (),
      );
  };

  module Get = {
    let stmt = "
SELECT
  uuid_of(issues_issues.uuid) as id,
  issues_issues.title as title,
  issues_issues.description as description,
  uuid_of(issues_boards.uuid) as board,
  uuid_of(users_users.uuid) as assignee,
  issues_issues.status as status
FROM issues_issues
LEFT JOIN users_users
ON users_users.id = issues_issues.assignee
LEFT JOIN issues_boards
ON issues_boards.id = issues_issues.board
WHERE issues_issues.uuid = UNHEX(REPLACE(?, '-', ''));
";

    [@decco]
    type parameters = string;

    let query = (connection, ~issueId) =>
      Sihl.App.Repo.getOne(
        ~connection,
        ~stmt,
        ~parameters=parameters_encode(issueId),
        ~decode=Model.Issue.t_decode,
        (),
      );
  };

  module Upsert = {
    let stmt = "
INSERT INTO issues_issues (
  uuid,
  title,
  description,
  board,
  assignee,
  status
) VALUES (
  UNHEX(REPLACE(?, '-', '')),
  ?,
  ?,
  (SELECT id FROM issues_boards WHERE issues_boards.uuid = UNHEX(REPLACE(?, '-', ''))),
  (SELECT id FROM users_users WHERE users_users.uuid = UNHEX(REPLACE(?, '-', ''))),
  ?
) ON DUPLICATE KEY UPDATE
title = VALUES(title),
description = VALUES(description),
board = VALUES(board),
assignee = VALUES(assignee),
status = VALUES(status)
;";

    [@decco]
    type parameters = (
      string,
      string,
      option(string),
      string,
      option(string),
      string,
    );

    let query = (connection, ~issue: Model.Issue.t) =>
      Sihl.App.Repo.execute(
        ~parameters=
          parameters_encode((
            issue.id,
            issue.title,
            issue.description,
            issue.board,
            issue.assignee,
            issue.status,
          )),
        connection,
        stmt,
      );
  };
};

module Board = {
  module Get = {
    let stmt = "
SELECT
  uuid_of(issues_boards.uuid) as id,
  issues_boards.title as title,
  uuid_of(users_users.uuid) as owner,
  issues_boards.status as status
FROM issues_boards
LEFT JOIN users_users
ON users_users.id  = issues_boards.owner
WHERE issues_boards.uuid = UNHEX(REPLACE(?, '-', ''));
";

    [@decco]
    type params = string;

    let query = (connection, ~boardId) =>
      Sihl.App.Repo.getOne(
        ~connection,
        ~stmt,
        ~parameters=params_encode(boardId),
        ~decode=Model.Board.t_decode,
        (),
      );
  };

  module GetAllByUser = {
    let stmt = "
SELECT
  uuid_of(issues_boards.uuid) as id,
  issues_boards.title as title,
  uuid_of(users_users.uuid) as owner,
  issues_boards.status as status
FROM issues_boards
LEFT JOIN users_users
ON users_users.id  = issues_boards.owner
WHERE users_users.uuid = UNHEX(REPLACE(?, '-', ''));
";

    [@decco]
    type params = string;

    let query = (connection, ~userId) =>
      Sihl.App.Repo.getMany(
        ~connection,
        ~stmt,
        ~decode=Model.Board.t_decode,
        ~parameters=params_encode(userId),
        (),
      );
  };

  module GetAll = {
    let stmt = "
SELECT
  uuid_of(issues_boards.uuid) as id,
  issues_boards.title as title,
  uuid_of(users_users.uuid) as owner,
  issues_boards.status as status
FROM issues_boards
LEFT JOIN users_users
ON users_users.id  = issues_boards.owner;
";

    let query = connection =>
      Sihl.App.Repo.getMany(
        ~connection,
        ~stmt,
        ~decode=Model.Board.t_decode,
        (),
      );
  };

  module Upsert = {
    let stmt = "
INSERT INTO issues_boards (
  uuid,
  title,
  owner,
  status
) VALUES (
  UNHEX(REPLACE(?, '-', '')),
  ?,
  (SELECT id FROM users_users WHERE users_users.uuid = UNHEX(REPLACE(?, '-', ''))),
  ?
)
ON DUPLICATE KEY UPDATE
title = VALUES(title),
owner = VALUES(owner),
status = VALUES(status)
;";

    [@decco]
    type parameters = (string, string, string, string);

    let query = (connection, ~board: Model.Board.t) =>
      Sihl.App.Repo.execute(
        ~parameters=
          parameters_encode((
            board.id,
            board.title,
            board.owner,
            board.status,
          )),
        connection,
        stmt,
      );
  };
};
