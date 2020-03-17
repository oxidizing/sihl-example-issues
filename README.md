[![CircleCI](https://circleci.com/gh/oxidizing/sihl-example-issues.svg?style=svg&circle-token=1351b6152d0a4c49555dada65d80fa977159028c)](https://circleci.com/gh/oxidizing/sihl-example-issues)

# Issue Management App

This is a simple full-stack app in Reason that uses the web framework [Sihl](https://github.com/oxidizing/sihl/) with [ReasonReact](https://reasonml.github.io/reason-react/).

You can check out the live demo here: [https://sihl-example-issues.oxidizing.io/](https://sihl-example-issues.oxidizing.io/)

![Screenshot](/images/screen3.png)

## Features

- login & register (thanks to [@sihl/users](https://www.npmjs.com/package/@sihl/users))
- creation of boards
- creation of issues on a board
- mark issues as *completed*

## Description

- Admin UI for users, boards and issues
- Uses backend models in the react client
- Optimistic rendering with error handling
- Client logic testable thanks to redux
- Integration tests with jest

It uses [@sihl/users](https://www.npmjs.com/package/@sihl/users) for user management.

## Getting started

Start a local MariaDB instance. You can use `docker-compose -f docker-compose.test.yml up -d` to quickly spin up the correct an instance with the right MariaDB version.

Run `yarn test` to run all tests.

Run `yarn sihl start` to start the web server. The admin UI can be accessed at `http://localhost:3000/admin/login/`.

## Tutorial

Check out the [tutorial](https://github.com/oxidizing/sihl/wiki/Tutorial:-Issue-management-app) in the Wiki.

## Screenshots

![Screenshot](/images/screen1.png)
![Screenshot](/images/screen2.png)
![Screenshot](/images/screen4.png)
