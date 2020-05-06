module Error = {
  let stringify = ({Decco.path, Decco.message, Decco.value}) =>
    "Failed to decode at location="
    ++ path
    ++ ", message="
    ++ message
    ++ ", json="
    ++ Js.Json.stringify(value);

  let stringifyResult = res => {
    switch (res) {
    | Ok(_) as ok => ok
    | Error(error) => Error(stringify(error))
    };
  };

  let stringifyDecoder = (decoder, json) => {
    switch (decoder(json)) {
    | Ok(_) as ok => ok
    | Error(error) => Error(stringify(error))
    };
  };
};

module Http = {
  module Msg = {
    [@decco]
    type t = {msg: string};
  };
};

let redirectIfNotAuthorized = response => {
  response->Async.let_(r => {
    let status = Fetch.Response.status(r);
    if (status === 401) {
      ReasonReactRouter.push("/app/login/");
    };
    response;
  });
};

let decodeRespone = (response, decode) => {
  Async.catchAsync(
    {
      let%Async json = Fetch.Response.json(response);
      Async.async(
        switch (decode(json)) {
        | Belt.Result.Ok(result) => Belt.Result.Ok(result)
        | Belt.Result.Error(error) =>
          Js.log(Error.stringify(error));
          Belt.Result.Error(
            "Invalid response retrieved url="
            ++ Fetch.Response.url(response)
            ++ " "
            ++ Error.stringify(error),
          );
        },
      );
    },
    _ => {
      Js.log(
        "Failed to parse response from url=" ++ Fetch.Response.url(response),
      );
      Async.async(
        Belt.Result.Error(
          "Failed request status="
          ++ string_of_int(Fetch.Response.status(response)),
        ),
      );
    },
  );
};

let decodeResult = (~decode, response) => {
  let%Async response = response;
  if (Fetch.Response.status(response) === 200) {
    decodeRespone(response, decode);
  } else {
    let%Async result = decodeRespone(response, Http.Msg.t_decode);
    Async.async(
      switch (result) {
      | Belt.Result.Ok(Http.Msg.{msg}) => Belt.Result.Error(msg)
      | Belt.Result.Error(_) as error => error
      },
    );
  };
};

let toResult = response => {
  let%Async response = response;
  if (Fetch.Response.status(response) === 200) {
    Async.async(Belt.Result.Ok());
  } else {
    let%Async result = decodeRespone(response, Http.Msg.t_decode);
    Async.async(
      switch (result) {
      | Belt.Result.Ok(Http.Msg.{msg}) => Belt.Result.Error(msg)
      | Belt.Result.Error(_) as error => error
      },
    );
  };
};

module Board = {
  module GetAll = {
    [@decco]
    type t = list(Model.Board.t);

    let f = () => {
      let ClientSession.{userId} = ClientSession.getOrThrow();

      Fetch.fetchWithInit(
        ClientConfig.baseUrl() ++ "/issues/users/" ++ userId ++ "/boards/",
        Fetch.RequestInit.make(
          ~method_=Get,
          ~headers=
            Fetch.HeadersInit.make({
              "authorization": "Bearer " ++ ClientSession.getOrThrow().token,
            }),
          (),
        ),
      )
      |> redirectIfNotAuthorized
      |> decodeResult(~decode=t_decode);
    };
  };

  module Add = {
    let f = (~title) => {
      let body = {j|{"title": "$(title)"}|j};
      Fetch.fetchWithInit(
        ClientConfig.baseUrl() ++ "/issues/boards/",
        Fetch.RequestInit.make(
          ~method_=Post,
          ~body=Fetch.BodyInit.make(body),
          ~headers=
            Fetch.HeadersInit.make({
              "authorization": "Bearer " ++ ClientSession.getOrThrow().token,
            }),
          (),
        ),
      )
      |> decodeResult(~decode=Model.Board.t_decode);
    };
  };

  module Issues = {
    [@decco]
    type t = list(Model.Issue.t);

    let f = (~boardId) => {
      Fetch.fetchWithInit(
        ClientConfig.baseUrl() ++ "/issues/boards/" ++ boardId ++ "/issues/",
        Fetch.RequestInit.make(
          ~method_=Get,
          ~headers=
            Fetch.HeadersInit.make({
              "authorization": "Bearer " ++ ClientSession.getOrThrow().token,
            }),
          (),
        ),
      )
      |> decodeResult(~decode=t_decode);
    };
  };
};

module Issue = {
  module Complete = {
    let f = (~issueId) => {
      Fetch.fetchWithInit(
        ClientConfig.baseUrl() ++ "/issues/issues/" ++ issueId ++ "/complete/",
        Fetch.RequestInit.make(
          ~method_=Post,
          ~headers=
            Fetch.HeadersInit.make({
              "authorization": ClientSession.getOrThrow().token,
            }),
          (),
        ),
      )
      |> toResult;
    };
  };

  module Add = {
    let f = (~boardId, ~title, ~description) => {
      let description =
        description
        ->Belt.Option.map(d => "\"" ++ d ++ "\"")
        ->Belt.Option.getWithDefault("null");
      let body = {j|
       {
         "board": "$(boardId)",
         "title": "$(title)",
         "description": $(description)
       }
       |j};
      Fetch.fetchWithInit(
        ClientConfig.baseUrl() ++ "/issues/issues/",
        Fetch.RequestInit.make(
          ~method_=Post,
          ~body=Fetch.BodyInit.make(body),
          ~headers=
            Fetch.HeadersInit.make({
              "authorization": ClientSession.getOrThrow().token,
            }),
          (),
        ),
      )
      |> decodeResult(~decode=Model.Issue.t_decode);
    };
  };
};

module User = {
  module Me = {
    let f = () => {
      Fetch.fetchWithInit(
        ClientConfig.baseUrl() ++ "/users/users/me/",
        Fetch.RequestInit.make(
          ~method_=Get,
          ~headers=
            Fetch.HeadersInit.make({
              "authorization": ClientSession.getOrThrow().token,
            }),
          (),
        ),
      )
      |> decodeResult(~decode=Model.User.t_decode);
    };
  };

  module ConfirmEmail = {
    let f = (~token) => {
      Fetch.fetch(
        ClientConfig.baseUrl() ++ "/users/confirm-email?token=" ++ token,
      )
      |> toResult;
    };
  };

  module RequestPasswordReset = {
    [@decco]
    type t = {email: string};

    let f = (~email) => {
      let body = {j|
       {
         "email": "$(email)"
       }
       |j};
      Fetch.fetchWithInit(
        ClientConfig.baseUrl() ++ "/users/request-password-reset",
        Fetch.RequestInit.make(
          ~method_=Post,
          ~body=Fetch.BodyInit.make(body),
          (),
        ),
      )
      |> toResult;
    };
  };

  module ResetPassword = {
    [@decco]
    type t = {
      token: string,
      new_password: string,
    };

    let f = (~token, ~newPassword) => {
      let body = {j|
       {
         "token": "$(token)",
         "newPassword": "$(newPassword)"
       }
       |j};
      Fetch.fetchWithInit(
        ClientConfig.baseUrl() ++ "/users/reset-password",
        Fetch.RequestInit.make(
          ~method_=Post,
          ~body=Fetch.BodyInit.make(body),
          (),
        ),
      )
      |> toResult;
    };
  };

  module Login = {
    [@decco]
    type t = {
      token: string,
      user_id: string,
    };

    let encode: string => string = [%raw "btoa"];
    let decode: string => string = [%raw "atob"];

    let f = (~email, ~password) => {
      let basicHeader = encode(email ++ ":" ++ password);
      Fetch.fetchWithInit(
        ClientConfig.baseUrl() ++ "/users/login/",
        Fetch.RequestInit.make(
          ~method_=Get,
          ~headers=
            Fetch.HeadersInit.make({
              "authorization": "Basic " ++ basicHeader,
            }),
          (),
        ),
      )
      |> decodeResult(~decode=t_decode);
    };
  };

  module Register = {
    [@decco]
    let f = (~username, ~givenName, ~familyName, ~email, ~password) => {
      let body = {j|
       {
         "email": "$(email)",
         "username": "$(username)",
         "password": "$(password)",
         "givenName": "$(givenName)",
         "familyName": "$(familyName)"
       }
       |j};

      Fetch.fetchWithInit(
        ClientConfig.baseUrl() ++ "/users/register/",
        Fetch.RequestInit.make(
          ~method_=Post,
          ~body=Fetch.BodyInit.make(body),
          (),
        ),
      )
      |> toResult;
    };
  };
};
