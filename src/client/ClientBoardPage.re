module Async = Sihl.Core.Async;

module Layout = ClientLayout;
module Login = ClientLoginPage;

module State = {
  type t = {
    boards: list(Model.Board.t),
    issues: list(Model.Issue.t),
  };

  let default = {boards: [], issues: []};

  let setIssues = (state, ~issues) => {
    {...state, issues};
  };

  let setBoards = (state, ~boards) => {
    {...state, boards};
  };

  let appendIssue = (state, ~issueId, ~boardId, ~title, ~description) => {
    let issue =
      Model.Issue.makeId(~id=issueId, ~board=boardId, ~title, ~description);
    let issues = Belt.List.concat(state.issues, [issue]);
    {...state, issues};
  };

  let updateIssue = (state, ~oldIssueId, ~issue: Model.Issue.t) => {
    let issues =
      state.issues
      ->Belt.List.keep(currentIssue => currentIssue.id !== oldIssueId)
      ->Belt.List.concat([issue]);
    {...state, issues};
  };

  let removeIssue = (state, ~issueId) => {
    {
      ...state,
      issues: Belt.List.keep(state.issues, issue => issue.id !== issueId),
    };
  };

  let setIssueStatus = (state, ~issueId, ~status) => {
    {
      ...state,
      issues:
        Belt.List.map(state.issues, issue =>
          issue.id === issueId ? Model.Issue.setStatus(issue, status) : issue
        ),
    };
  };

  let completeIssue = (state, ~issueId) => {
    {
      ...state,
      issues:
        Belt.List.map(state.issues, issue =>
          issue.id === issueId ? Model.Issue.complete(issue) : issue
        ),
    };
  };

  let addBoard = (state, ~boardId, ~title) => {
    {
      ...state,
      boards:
        Belt.List.concat(
          state.boards,
          [Model.Board.makeId(~id=boardId, ~title, ~owner="")],
        ),
    };
  };

  let updateBoard = (state, ~oldBoardId, ~board: Model.Board.t) => {
    let boards =
      state.boards
      ->Belt.List.keep(currentBoard => currentBoard.id !== oldBoardId)
      ->Belt.List.concat([board]);
    {...state, boards};
  };

  let removeBoard = (state, ~boardId) => {
    {
      ...state,
      boards: Belt.List.keep(state.boards, board => board.id !== boardId),
    };
  };
};

module Action = {
  type t =
    | StartAddIssue(string, string, string, option(string))
    | SucceedAddIssue(string, Model.Issue.t)
    | FailAddIssue(string)
    | StartCompleteIssue(string)
    | FailCompleteIssue(string, string)
    | SetIssues(list(Model.Issue.t))
    | StartAddBoard(string, string)
    | SucceedAddBoard(string, Model.Board.t)
    | FailAddBoard(string)
    | SetBoards(list(Model.Board.t));

  let reducer = (state, action) =>
    switch (state, action) {
    | (state, StartAddIssue(issueId, boardId, title, description)) =>
      State.appendIssue(state, ~issueId, ~title, ~description, ~boardId)
    | (state, SucceedAddIssue(oldIssueId, issue)) =>
      State.updateIssue(state, ~oldIssueId, ~issue)
    | (state, FailAddIssue(issueId)) => State.removeIssue(state, ~issueId)
    | (state, StartCompleteIssue(issueId)) =>
      State.completeIssue(state, ~issueId)
    | (state, FailCompleteIssue(issueId, status)) =>
      State.setIssueStatus(state, ~issueId, ~status)
    | (state, StartAddBoard(boardId, title)) =>
      State.addBoard(state, ~boardId, ~title)
    | (state, SucceedAddBoard(oldBoardId, board)) =>
      ReasonReactRouter.push("/app/boards/" ++ board.id);
      State.updateBoard(state, ~oldBoardId, ~board);
    | (state, FailAddBoard(boardId)) =>
      ReasonReactRouter.push("/app/boards/");
      State.removeBoard(state, ~boardId);
    | (state, SetIssues(issues)) => State.setIssues(state, ~issues)
    | (state, SetBoards(boards)) => State.setBoards(state, ~boards)
    };
};

module AddBoard = {
  [@react.component]
  let make = (~dispatch) => {
    let (title, setTitle) = React.useState(_ => "");
    let (_, setError) =
      React.useContext(ClientContextProvider.Error.context);

    <div className="field has-addons">
      <div className="control">
        <input
          onChange={event => {
            let value = ReactEvent.Form.target(event)##value;
            setTitle(_ => value);
          }}
          value=title
          className="input"
          type_="text"
          placeholder="Board title"
        />
      </div>
      <div className="control">
        <a
          className="button is-info"
          onClick={event => {
            ReactEvent.Mouse.preventDefault(event);
            {let boardId = Sihl.Core.Uuid.V4.uuidv4();
             dispatch(Action.StartAddBoard(boardId, title));
             let%Async result = ClientApi.Board.Add.f(~title);
             Async.async(
               switch (result) {
               | Ok(board) =>
                 setTitle(_ => "");
                 dispatch(Action.SucceedAddBoard(boardId, board));
               | Error(msg) =>
                 setError(_ => Some("Failed create board: " ++ msg));
                 dispatch(Action.FailAddBoard(boardId));
               },
             )}
            ->ignore;
          }}>
          {React.string("Add board")}
        </a>
      </div>
    </div>;
  };
};

module SelectBoard = {
  let selectedBoard = () => {
    let url = ReasonReactRouter.useUrl();
    switch (url.path) {
    | ["app", "boards", boardId] => Some(boardId)
    | _ => None
    };
  };

  [@react.component]
  let make = (~boards) => {
    <div className="field">
      <p className="control">
        <span className="select">
          <select
            value={selectedBoard()->Belt.Option.getWithDefault("select")}
            onChange={event => {
              let value = ReactEvent.Form.target(event)##value;
              if (value === "select") {
                ReasonReactRouter.push("/app/boards/");
              } else {
                ReasonReactRouter.push("/app/boards/" ++ value);
              };
            }}>
            <option value="select"> {React.string("Select board")} </option>
            {boards
             ->Belt.List.map((board: Model.Board.t) =>
                 <option key={board.id} value={board.id}>
                   {React.string(board.title)}
                 </option>
               )
             ->Belt.List.toArray
             ->React.array}
          </select>
        </span>
      </p>
    </div>;
  };
};

module Boards = {
  [@react.component]
  let make = (~dispatch, ~boards) => {
    <div> <AddBoard dispatch /> <SelectBoard boards /> </div>;
  };
};

module Issue = {
  let complete = (setError, dispatch, ~issueId, ~currentStatus) => {
    {
      dispatch(Action.StartCompleteIssue(issueId));
      let%Async result = ClientApi.Issue.Complete.f(~issueId);
      Async.async(
        switch (result) {
        | Belt.Result.Ok () => ()
        | Belt.Result.Error(msg) =>
          dispatch(Action.FailCompleteIssue(issueId, currentStatus));
          setError(_ => Some("Failed create issue: " ++ msg));
        },
      );
    }
    ->ignore;
  };

  [@react.component]
  let make = (~issue: Model.Issue.t, ~dispatch) => {
    let (_, setError) =
      React.useContext(ClientContextProvider.Error.context);
    let complete = complete(setError, dispatch);

    let statusBadge =
      issue.status === "todo"
        ? <span className="tag is-pulled-right">
            {React.string("To do")}
          </span>
        : <span className="tag is-success is-pulled-right">
            {React.string("Completed")}
          </span>;

    <div className="box" style={ReactDOMRe.Style.make(~minHeight="8em", ())}>
      statusBadge
      <h4 className="is-4 title"> {React.string(issue.title)} </h4>
      {issue.description
       ->Belt.Option.map(description =>
           <span> {React.string(description)} </span>
         )
       ->Belt.Option.getWithDefault(React.null)}
      {issue.status === "todo"
         ? <button
             className="button is-small is-info is-pulled-right"
             onClick={_ =>
               complete(~issueId=issue.id, ~currentStatus=issue.status)
             }>
             {React.string("Complete")}
           </button>
         : React.null}
    </div>;
  };
};

module Issues = {
  [@react.component]
  let make = (~issues: list(Model.Issue.t), ~dispatch) => {
    <div>
      {Belt.List.length(issues) === 0
         ? <span> {React.string("No issues found")} </span>
         : issues
           ->Belt.List.map(issue => <Issue key={issue.id} issue dispatch />)
           ->Belt.List.toArray
           ->React.array}
    </div>;
  };
};

module Board = {
  [@react.component]
  let make = (~boardId, ~issues, ~dispatch) => {
    let (_, setError) =
      React.useContext(ClientContextProvider.Error.context);

    React.useEffect1(
      () => {
        {
          let%Async result = ClientApi.Board.Issues.f(~boardId);
          Async.async(
            switch (result) {
            | Ok(issues) => dispatch(Action.SetIssues(issues))
            | Error(msg) => setError(_ => Some(msg))
            },
          );
        }
        ->ignore;
        None;
      },
      [|boardId|],
    );

    switch (issues) {
    | Some(issues) => <Issues issues dispatch />
    | None => <span> {React.string("Loading...")} </span>
    };
  };
};

module AddIssue = {
  let addIssue = (setError, dispatch, ~boardId, ~title, ~description) => {
    let issueId = Sihl.Core.Uuid.V4.uuidv4();
    dispatch(Action.StartAddIssue(issueId, boardId, title, description));
    let%Async result = ClientApi.Issue.Add.f(~boardId, ~title, ~description);
    Async.async(
      switch (result) {
      | Belt.Result.Ok(issue) => dispatch(SucceedAddIssue(issueId, issue))
      | Belt.Result.Error(msg) =>
        setError(_ => Some("Failed create issue: " ++ msg));
        dispatch(Action.FailAddIssue(issueId));
      },
    );
  };

  [@react.component]
  let make = (~boardId, ~dispatch) => {
    let (title, setTitle) = React.useState(_ => "");
    let (description, setDescription) = React.useState(_ => None);
    let (_, setError) =
      React.useContext(ClientContextProvider.Error.context);

    <div style={ReactDOMRe.Style.make(~marginBottom="2em", ())}>
      <div className="field">
        <div className="control">
          <input
            onChange={event => {
              let value = ReactEvent.Form.target(event)##value;
              setTitle(_ => value);
            }}
            value=title
            className="input"
            type_="text"
            placeholder="Issue title"
          />
        </div>
      </div>
      <div className="field">
        <div className="control">
          <textarea
            onChange={event => {
              let value = ReactEvent.Form.target(event)##value;
              setDescription(_ => value);
            }}
            value={description->Belt.Option.getWithDefault("")}
            className="textarea"
            placeholder="Description"
          />
        </div>
      </div>
      <button
        className="button is-info"
        onClick={event => {
          ReactEvent.Mouse.preventDefault(event);
          let issueId = Sihl.Core.Uuid.V4.uuidv4();
          dispatch(
            Action.StartAddIssue(issueId, boardId, title, description),
          );
          {let%Async result =
             ClientApi.Issue.Add.f(~boardId, ~title, ~description);
           Async.async(
             switch (result) {
             | Belt.Result.Ok(issue) =>
               setTitle(_ => "");
               setDescription(_ => None);
               dispatch(SucceedAddIssue(issueId, issue));
             | Belt.Result.Error(msg) =>
               setError(_ => Some("Failed create issue: " ++ msg));
               dispatch(Action.FailAddIssue(issueId));
             },
           )}
          ->ignore;
        }}>
        {React.string("Add Issue")}
      </button>
    </div>;
  };
};

[@react.component]
let make = () => {
  let (state, dispatch) = React.useReducer(Action.reducer, State.default);
  let (_, setError) = React.useContext(ClientContextProvider.Error.context);

  React.useEffect1(
    () => {
      {
        let%Async boards = ClientApi.Board.GetAll.f();
        Async.async(
          switch (boards) {
          | Ok(boards) => dispatch(SetBoards(boards))
          | Error(msg) => setError(_ => Some(msg))
          },
        );
      }
      ->ignore;
      None;
    },
    [||],
  );

  <Layout>
    <div className="columns">
      <div className="column is-one-third">
        <Boards dispatch boards={state.boards} />
      </div>
      <div className="column is-one-third">
        <h2 className="title is-2"> {React.string("Issues")} </h2>
        {switch (SelectBoard.selectedBoard()) {
         | None => <span> {React.string("Please select a board")} </span>
         | Some(boardId) =>
           <div>
             <AddIssue dispatch boardId />
             <Board issues={Some(state.issues)} dispatch boardId />
           </div>
         }}
      </div>
    </div>
  </Layout>;
};
