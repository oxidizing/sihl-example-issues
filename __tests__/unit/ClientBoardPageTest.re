open Jest;
open Expect;

describe("ClientBoardPage", () => {
  test("add issue", () => {
    let state = ClientBoardPage.State.default;
    let actual =
      ClientBoardPage.Action.reducer(
        state,
        ClientBoardPage.Action.StartAddIssue(
          "0",
          "1",
          "title",
          Some("description"),
        ),
      );

    actual.issues
    |> expect
    |> toEqual([
         Model.Issue.makeId(
           ~id="0",
           ~title="title",
           ~description=Some("description"),
           ~board="1",
         ),
       ]);
  })
});
