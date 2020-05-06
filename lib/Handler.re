open Base;
let ( let* ) = Lwt.bind;

module GetBoardsByUser = {
  open Sihl_core;
  [@deriving yojson]
  type body_out = list(Model.Board.t);

  let handler =
    Http.get("/issues/users/:id/boards/", req => {
      let user_id = Http.param(req, "id");
      let user = Sihl_users.Middleware.Authn.authenticate(req);
      let* response = Service.Board.get_all_by_user(req, user, ~user_id);
      response
      |> body_out_to_yojson
      |> Yojson.Safe.to_string
      |> Http.Response.json
      |> Lwt.return;
    });
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
  [@deriving yojson]
  type body_in = {title: string};

  [@deriving yojson]
  type body_out = Model.Board.t;

  let handler =
    Http.post("/issues/boards/", req => {
      let user = Sihl_users.Middleware.Authn.authenticate(req);
      let* {title} = Sihl_core.Http.require_body_exn(req, body_in_of_yojson);
      let* response = Service.Board.create(req, user, ~title);
      response
      |> body_out_to_yojson
      |> Yojson.Safe.to_string
      |> Http.Response.json
      |> Lwt.return;
    });
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
