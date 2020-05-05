module GetBoardsByUser = {
  open Sihl_core;
  let handler = Http.get("", _ => Http.Response.empty |> Lwt.return);
  /* [@decco] */
  /* type body_out = list(Model.Board.t); */
  /* [@decco] */
  /* type params = {userId: string}; */
  /* let endpoint = (root, database) => */
  /*   Sihl.App.Http.dbEndpoint({ */
  /*     database, */
  /*     verb: GET, */
  /*     path: {j|/$root/users/:userId/boards/|j}, */
  /*     handler: (conn, req) => { */
  /*       open! Sihl.App.Http.Endpoint; */
  /*       let%Async token = Sihl.App.Http.requireAuthorizationToken(req); */
  /*       let%Async user = Sihl.Users.User.authenticate(conn, token); */
  /*       let%Async {userId} = req.requireParams(params_decode); */
  /*       let%Async boards = Service.Board.getAllByUser((conn, user), ~userId); */
  /*       let response = */
  /*         boards |> Sihl.Common.Db.Result.Query.rows |> body_out_encode; */
  /*       Async.async @@ Sihl.App.Http.Endpoint.OkJson(response); */
  /*     }, */
  /*   }); */
};

module GetIssuesByBoard = {
  open Sihl_core;
  let handler = Http.get("", _ => Http.Response.empty |> Lwt.return);
  /* [@decco] */
  /* type body_out = list(Model.Issue.t); */
  /* [@decco] */
  /* type params = {boardId: string}; */
  /* let endpoint = (root, database) => */
  /*   Sihl.App.Http.dbEndpoint({ */
  /*     database, */
  /*     verb: GET, */
  /*     path: {j|/$root/boards/:boardId/issues/|j}, */
  /*     handler: (conn, req) => { */
  /*       open! Sihl.App.Http.Endpoint; */
  /*       let%Async token = Sihl.App.Http.requireAuthorizationToken(req); */
  /*       let%Async user = Sihl.Users.User.authenticate(conn, token); */
  /*       let%Async {boardId} = req.requireParams(params_decode); */
  /*       let%Async issues = */
  /*         Service.Issue.getAllByBoard((conn, user), ~boardId); */
  /*       let response = */
  /*         issues |> Sihl.Common.Db.Result.Query.rows |> body_out_encode; */
  /*       Async.async @@ Sihl.App.Http.Endpoint.OkJson(response); */
  /*     }, */
  /*   }); */
};

module AddBoard = {
  open Sihl_core;
  let handler = Http.get("", _ => Http.Response.empty |> Lwt.return);
  /* [@decco] */
  /* type body_in = {title: string}; */
  /* [@decco] */
  /* type body_out = Model.Board.t; */
  /* let endpoint = (root, database) => */
  /*   Sihl.App.Http.dbEndpoint({ */
  /*     database, */
  /*     verb: POST, */
  /*     path: {j|/$root/boards/|j}, */
  /*     handler: (conn, req) => { */
  /*       open! Sihl.App.Http.Endpoint; */
  /*       let%Async token = Sihl.App.Http.requireAuthorizationToken(req); */
  /*       let%Async user = Sihl.Users.User.authenticate(conn, token); */
  /*       let%Async {title} = req.requireBody(body_in_decode); */
  /*       let%Async board = Service.Board.create((conn, user), ~title); */
  /*       Async.async @@ OkJson(body_out_encode(board)); */
  /*     }, */
  /*   }); */
};

module AddIssue = {
  open Sihl_core;
  let handler = Http.get("", _ => Http.Response.empty |> Lwt.return);
  /* [@decco] */
  /* type body_in = { */
  /*   title: string, */
  /*   description: option(string), */
  /*   board: string, */
  /* }; */
  /* [@decco] */
  /* type body_out = Model.Issue.t; */
  /* let endpoint = (root, database) => */
  /*   Sihl.App.Http.dbEndpoint({ */
  /*     database, */
  /*     verb: POST, */
  /*     path: {j|/$root/issues/|j}, */
  /*     handler: (conn, req) => { */
  /*       open! Sihl.App.Http.Endpoint; */
  /*       let%Async token = Sihl.App.Http.requireAuthorizationToken(req); */
  /*       let%Async user = Sihl.Users.User.authenticate(conn, token); */
  /*       let%Async {title, description, board} = */
  /*         req.requireBody(body_in_decode); */
  /*       let%Async issue = */
  /*         Service.Issue.create((conn, user), ~title, ~description, ~board); */
  /*       Async.async @@ OkJson(body_out_encode(issue)); */
  /*     }, */
  /*   }); */
};

module CompleteIssue = {
  open Sihl_core;
  let handler = Http.get("", _ => Http.Response.empty |> Lwt.return);
  /* [@decco] */
  /* type params = {issueId: string}; */
  /* [@decco] */
  /* type body_out = {message: string}; */
  /* let endpoint = (root, database) => */
  /*   Sihl.App.Http.dbEndpoint({ */
  /*     database, */
  /*     verb: POST, */
  /*     path: {j|/$root/issues/:issueId/complete/|j}, */
  /*     handler: (conn, req) => { */
  /*       open! Sihl.App.Http.Endpoint; */
  /*       let%Async token = Sihl.App.Http.requireAuthorizationToken(req); */
  /*       let%Async user = Sihl.Users.User.authenticate(conn, token); */
  /*       let%Async {issueId} = req.requireParams(params_decode); */
  /*       let%Async _ = Service.Issue.complete((conn, user), ~issueId); */
  /*       Async.async @@ OkJson(body_out_encode({message: "ok"})); */
  /*     }, */
  /*   }); */
};

module Client = {
  open Sihl_core;
  let handler = Http.get("", _ => Http.Response.empty |> Lwt.return);
  /* module Asset = { */
  /*   [@decco] */
  /*   type params = {asset: string}; */
  /*   let endpoint = () => */
  /*     Sihl.App.Http.endpoint({ */
  /*       verb: GET, */
  /*       path: {j|/asset/:asset|j}, */
  /*       handler: req => { */
  /*         open! Sihl.App.Http.Endpoint; */
  /*         let%Async {asset} = req.requireParams(params_decode); */
  /*         Async.async @@ Sihl.App.Http.Endpoint.OkFile("dist/" ++ asset); */
  /*       }, */
  /*     }); */
  /* }; */
  /* module App = { */
  /*   let endpoint = () => */
  /*     Sihl.App.Http.endpoint({ */
  /*       verb: GET, */
  /*       path: {j|(/|/app|/app/\*)|j}, */
  /*       handler: _ => { */
  /*         open! Sihl.App.Http.Endpoint; */
  /*         Async.async @@ Sihl.App.Http.Endpoint.OkFile("dist/index.html"); */
  /*       }, */
  /*     }); */
  /* }; */
};

module AdminUi = {
  module Issues = {
    open Sihl_core;
    let handler = Http.get("", _ => Http.Response.empty |> Lwt.return);
    /* let endpoint = (root, database) => */
    /*   Sihl.App.Http.dbEndpoint({ */
    /*     database, */
    /*     verb: GET, */
    /*     path: {j|/admin/$root/issues/|j}, */
    /*     handler: (conn, req) => { */
    /*       open! Sihl.App.Http.Endpoint; */
    /*       let%Async token = */
    /*         Sihl.App.Http.requireSessionCookie(req, "/admin/login/"); */
    /*       let%Async user = Sihl.Users.User.authenticate(conn, token); */
    /*       let%Async issues = Service.Issue.getAll((conn, user)); */
    /*       let issues = issues |> Sihl.Common.Db.Result.Query.rows; */
    /*       Async.async @@ */
    /*       OkHtml(Sihl.Users.AdminUi.render(<AdminUi.Issues issues />)); */
    /*     }, */
    /*   }); */
  };

  module Boards = {
    open Sihl_core;
    let handler = Http.get("", _ => Http.Response.empty |> Lwt.return);
    /* let endpoint = (root, database) => */
    /*   Sihl.App.Http.dbEndpoint({ */
    /*     database, */
    /*     verb: GET, */
    /*     path: {j|/admin/$root/boards/|j}, */
    /*     handler: (conn, req) => { */
    /*       open! Sihl.App.Http.Endpoint; */
    /*       let%Async token = */
    /*         Sihl.App.Http.requireSessionCookie(req, "/admin/login/"); */
    /*       let%Async user = Sihl.Users.User.authenticate(conn, token); */
    /*       let%Async boards = Service.Board.getAll((conn, user)); */
    /*       let boards = boards |> Sihl.Common.Db.Result.Query.rows; */
    /*       Async.async @@ */
    /*       OkHtml(Sihl.Users.AdminUi.render(<AdminUi.Boards boards />)); */
    /*     }, */
    /*   }); */
  };
};
