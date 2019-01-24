# SQLDrivers

Alex Davies-Moore edited this page 15 days ago · [9 revisions](https://github.com/golang/go/wiki/SQLDrivers/_history)

### 

###  Pages 97

# Contents

- [Home](https://github.com/golang/go/wiki/)
- [Getting started with Go](https://github.com/golang/go/wiki/#getting-started-with-go)
- [Working with Go](https://github.com/golang/go/wiki/#working-with-go)
- [Learning more about Go](https://github.com/golang/go/wiki/#learning-more-about-go)
- [The Go Community](https://github.com/golang/go/wiki/#the-go-community)
- [Using the go toolchain](https://github.com/golang/go/wiki/#using-the-go-toolchain)
- [Additional Go Programming Wikis](https://github.com/golang/go/wiki/#additional-go-programming-wikis)
- [Online Services that work with Go](https://github.com/golang/go/wiki/#online-services-that-work-with-go)
- [Troubleshooting Go Programs in Production](https://github.com/golang/go/wiki/#troubleshooting-go-programs-in-production)
- [Contributing to the Go Project](https://github.com/golang/go/wiki/#contributing-to-the-go-project)
- [Platform Specific Information](https://github.com/golang/go/wiki/#platform-specific-information)
- [Release Specific Information](https://github.com/golang/go/wiki/#release-specific-information)

##### Clone this wiki locally

# 

# SQL database drivers

The database/sql and database/sql/driver packages are designed for using databases from Go and implementing database drivers, respectively.

See the design goals doc:

> <http://golang.org/src/pkg/database/sql/doc.txt>

# 

# Drivers

Drivers for Go's sql package include:

- **Couchbase N1QL**: <https://github.com/couchbaselabs/go_n1ql>
- **DB2**: <https://bitbucket.org/phiggins/db2cli>
- **Firebird SQL**: <https://github.com/nakagami/firebirdsql>
- **MS ADODB**: <https://github.com/mattn/go-adodb>
- **MS SQL Server** (pure go): <https://github.com/denisenkom/go-mssqldb>
- **MySQL**: <https://github.com/ziutek/mymysql> `[*]`
- **MySQL**: <https://github.com/go-sql-driver/mysql/> `[*]`
- **ODBC**: <https://bitbucket.org/miquella/mgodbc>
- **ODBC**: <https://github.com/alexbrainman/odbc>
- **Oracle**: <https://github.com/mattn/go-oci8>
- **Oracle**: <https://github.com/rana/ora>
- **QL**: <http://godoc.org/github.com/cznic/ql/driver>
- **Postgres** (pure Go): <https://github.com/lib/pq> `[*]`
- **Postgres** (uses cgo): <https://github.com/jbarham/gopgsqldriver>
- **Postgres** (pure Go): <https://github.com/jackc/pgx>
- **SAP HANA** (pure go): <https://github.com/SAP/go-hdb>
- **SQLite**: <https://github.com/mattn/go-sqlite3> `[*]`
- **SQLite**: <https://github.com/mxk/go-sqlite>
- **Sybase SQL Anywhere**: <https://github.com/a-palchikov/sqlago>
- **YQL (Yahoo! Query Language)**: <https://github.com/mattn/go-yql>

Drivers marked with a `[*]` are both included in and pass the compatibility test suite at<https://github.com/bradfitz/go-sql-test>

来源： <<https://github.com/golang/go/wiki/SQLDrivers>>