import os

from datetime import datetime
from sqlalchemy import create_engine, Table, text, MetaData, select, insert, func, update
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# create engine with SQLAlchemy
engine = create_engine("sqlite+pysqlite:///finance.db", echo=True)

# create metadata object
metadata_obj = MetaData()

# create table object
user_table = Table("users", metadata_obj, autoload_with=engine)
history_table = Table("history", metadata_obj, autoload_with=engine)


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """home page"""
    history = []  # initialize user's history list of dict
    sum = 0  # initialize user's total to be displayed later
    with engine.begin() as conn:
        # get user balance
        user_balance = conn.execute(
            select(user_table.c.cash)
            .where(user_table.c.id == session["user_id"])
        ).scalar_one()

        # get the symbols and shares of user grouped by symbols
        results = conn.execute(
            select(
                history_table.c.symbol,
                func.sum(history_table.c.amount).label("shares")
            )
            .where(history_table.c.userid == session["user_id"]).group_by(history_table.c.symbol)
        )

        # iterate through every row in the results object to get the symbol and amount of shares out of it
        for row in results:
            symbol = row.symbol
            shares = row.shares

            # get dictionary quote with the stocks information
            quote = lookup(symbol)

            # get only the price of the current stock
            price = quote["price"]
            total = price * shares

            # sum every stock's value
            sum = sum + total

            # append to history list the dictionaries of each stock
            history.append({
                "symbol": symbol,
                "shares": shares,
                "price": price,
                "total": total
            })
        print(history)
        return render_template("index.html", history=history, cash=user_balance, total=sum+float(user_balance))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """buy stocks"""
    # if user submitted a POST request
    if request.method == "POST":
        stock = lookup(request.form.get("symbol"))

        # check if stock was found by lookup()
        if not stock:
            return apology("Could not find symbol", 400)

        # try to convert form to int, if not possible, user inputed invalid input
        try:
            shares = int(request.form.get("shares"))
        except:
            return apology("invalid amount of shares", 400)

        # check if user inputed positive integer
        if shares < 0:
            return apology("invalid amount of shares", 400)

        user_balance = 0
        with engine.begin() as conn:
            # get user balance
            user_balance = conn.execute(
                select(user_table.c.cash)
                .where(user_table.c.id == session["user_id"])
            ).scalar_one()

            # check if user is trying to buy more stocks than can afford
            if user_balance < float(shares) * float(stock["price"]):
                return apology("not enough funds", 400)

            # insert transaction to database
            result = conn.execute(
                insert(history_table)
                .values(userid=session["user_id"], symbol=stock["symbol"],
                        company=stock["name"], type="buy", amount=shares,
                        price=stock["price"], time=datetime.now())
            )

            # update user's balance based on how much they spent
            update_result = conn.execute(
                update(user_table)
                .where(user_table.c.id == session["user_id"])
                .values(cash=float(user_balance) - (float(shares) * stock["price"]))
            )

            flash("Bought!")
            return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """show user history of transactions"""
    # initialize history list
    history = []
    with engine.begin() as conn:
        # get result object with rows of transaction information
        results = conn.execute(
            select(
                history_table.c.symbol,
                history_table.c.amount,
                history_table.c.price,
                history_table.c.time
            ).where(history_table.c.userid == session["user_id"]).order_by(history_table.c.time.desc())
        )

        # iterate through result object getting separate results appending to the list of dictionaries history
        for row in results:
            symbol = row.symbol
            shares = row.amount
            price = row.price
            time = row.time

            history.append({
                "symbol": symbol,
                "shares": shares,
                "price": price,
                "time": time
            })

    return render_template("history.html", history=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        with engine.begin() as conn:
            row = conn.execute(
                select(user_table)
                .where(user_table.c.username == request.form.get("username"))
            ).fetchone()

            # Ensure username exists and password is correct
            if row == None or not check_password_hash(
                row[2], request.form.get("password")
            ):
                return apology("invalid username and/or password", 403)

            # Remember which user has logged in
            session["user_id"] = row[0]

        # Redirect user to home page
        flash("Logged in!")
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """quote for stock's symbol"""
    # if user submitted request via POST
    if request.method == "POST":
        # store the list from lookup
        company = lookup(request.form.get("symbol"))

        # check if lookup returned a valid result
        if not company:
            return apology("Could not find symbol", 400)
        else:
            # return template with stock's quoted information
            return render_template("quoted.html", company=company)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        # check if user inputed username
        if not request.form.get("username"):
            return apology("Must register a username", 400)

        # ensure user entered a password
        if not request.form.get("password"):
            return apology("Register a password", 400)

        # ensure password and confirmation match
        if request.form.get("password") != request.form.get("confirmation"):
            return apology("Password and confirmation do not match", 400)

        with engine.begin() as conn:
            # check for usernames already in the database
            row = conn.execute(
                select(user_table)
                .where(user_table.c.username == request.form.get("username"))
            ).fetchone()

            if row != None:
                if request.form.get("username") == row[1]:
                    return apology("Username already used", 400)

            # submitting system to insert username and password hashed to database
            result = conn.execute(
                insert(user_table)
                .values(username=request.form.get("username"),
                        hash=generate_password_hash(request.form.get("password")))
            )

        flash("Registered!")
        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """sell stock"""
    # if user submitted request via POST
    if request.method == "POST":
        # initialize variable symbol_to_sell and check if it was inputed
        symbol_to_sell = request.form.get("symbol")
        if not symbol_to_sell:
            return apology("provide a symbol", 400)

        # initialize variable shares_to_sell and check validity
        try:
            shares_to_sell = int(request.form.get("shares"))
        except:
            return apology("invalid amount of shares", 400)

        if not shares_to_sell or shares_to_sell < 1:
            return apology("invalid amount of shares", 400)

        # get current price of stock inputed
        quote = lookup(symbol_to_sell)
        current_price = quote["price"]

        with engine.begin() as conn:
            # get user's current balance
            user_balance = conn.execute(
                select(user_table.c.cash)
                .where(user_table.c.id == session["user_id"])
            ).scalar_one()

            # get amount of shares owned by the user of the specific stock choosen
            shares_owned = conn.execute(
                select(func.sum(history_table.c.amount).label("shares"))
                .where(history_table.c.userid == session["user_id"])
                .where(history_table.c.symbol == symbol_to_sell).group_by(history_table.c.symbol)
            ).scalar_one()

            # check if user is trying to sell more shares than they own
            if shares_owned < int(shares_to_sell):
                return apology("you don't own that many shares", 400)
            else:
                # execute insertion of sell transaction to history table
                result = conn.execute(
                    insert(history_table)
                    .values(userid=session["user_id"], symbol=symbol_to_sell,
                            company=quote["name"], type="sell", amount=-shares_to_sell,
                            price=current_price, time=datetime.now())
                )

                # execute update of user's balance
                update_result = conn.execute(
                    update(user_table)
                    .where(user_table.c.id == session["user_id"])
                    .values(cash=float(user_balance) + (float(shares_to_sell) * current_price))
                )

        flash("Sold!")
        return redirect("/")

    # if user got to page with a GET request
    else:
        # initialize options list to be displayed
        options = []
        with engine.begin() as conn:
            # get user's current balance
            user_balance = conn.execute(
                select(user_table.c.cash)
                .where(user_table.c.id == session["user_id"])
            ).scalar_one()

            # get result object with user's symbols of stocks owned
            results = conn.execute(
                select(history_table.c.symbol)
                .where(history_table.c.userid == session["user_id"]).group_by(history_table.c.symbol)
            )

            # append symbols to options list
            for row in results:
                symbol = row.symbol
                options.append(symbol)

            return render_template("sell.html", options=options)


@app.route("/change_password", methods=["GET", "POST"])
@login_required
def change_password():
    """changes user password"""
    # if user submitted request via POST
    if request.method == "POST":
        # initialize variables with forms submitted
        old_password = request.form.get("old_password")
        new_password = request.form.get("new_password")
        confirmation = request.form.get("confirmation")

        # check for input validity
        if not old_password or not new_password or not confirmation:
            return apology("invalid password", 400)
        if new_password != confirmation:
            return apology("password confirmation don't match", 400)
        if old_password == new_password:
            return apology("new password must be different")

        with engine.begin() as conn:
            # get user's old password hashed
            hashed_old_password = conn.execute(
                select(user_table.c.hash)
                .where(user_table.c.id == session["user_id"])
            ).scalar_one()

            # check if old_password inputed matches the one in the database
            if not check_password_hash(hashed_old_password, old_password):
                return apology("Old password is not correct", 400)

            # executes change of password to database
            result = conn.execute(
                update(user_table)
                .where(user_table.c.id == session["user_id"])
                .values(hash=generate_password_hash(new_password))
            )

        flash("Password Changed!")
        return redirect("/")

    else:
        return render_template("change_password.html")
