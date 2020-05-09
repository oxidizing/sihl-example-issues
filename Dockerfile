FROM node:10.16.3 AS js-builder
WORKDIR /home/sihl/app
COPY package.json yarn.lock bsconfig.json ./
COPY src src
COPY static static
RUN yarn
RUN yarn build

FROM ocaml/opam2:alpine AS ocaml-builder
WORKDIR /home/opam/app
USER root
# https://unix.stackexchange.com/questions/578949/sudo-setrlimitrlimit-core-operation-not-permitted
RUN echo "Set disable_coredump false" >> /etc/sudo.conf
# TODO why do we need that at compile time?
RUN apk add mariadb-dev
USER opam
COPY sihl_example_issues.opam Makefile dune-project ./
COPY src src
RUN sudo chown -R opam .
RUN opam remote remove --all default && \
        opam remote add default https://opam.ocaml.org
RUN opam pin add -y -n sihl_example_issues . && \
        opam pin add -y -n sihl_core https://github.com/oxidizing/sihl.git\#0.0.20 && \
        opam pin add -y -n sihl_users https://github.com/oxidizing/sihl.git\#0.0.20
RUN opam depext -y sihl_example_issues
RUN opam install --deps-only -y sihl_example_issues
RUN opam config exec -- make

FROM alpine:latest
WORKDIR /app
# TODO use opam depext |> file to automatically install all systems deps
RUN apk --update add emacs-nox libffi-dev linux-headers m4 pcre-dev mariadb-dev postgresql-dev
COPY --from=ocaml-builder /home/opam/app/_build/default/src/bin/Run.exe run.exe
COPY --from=js-builder /home/sihl/app/dist static
EXPOSE 3000
CMD ["./run.exe", "start"]
