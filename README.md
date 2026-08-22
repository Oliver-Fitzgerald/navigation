# navigate

A command that returns a list of frequently entered directories that you can jump to

## Set Up

### Build Instructions

```bash
cd build;
```

```bash
cmake ../;
```

```bash
make;
```

### Configuring Bash Environment

Add the following commands and alias to your .bashrc or .bash_alias:

Tool execution alias: 
```bash
alias nav="navigation;navigation_cmd;"
```

Bash pre-start:
```bash
startup() {
    history -c;
    rm ~/.history
    touch ~/.history
    . /home/ollie/projects/personal/navigation/src/main/resources/recent_stops_initialize.sh
}
startup
```
Bash post-start:
```bash
cleanup() {
    history -a ~/.history;
    /home/ollie/projects/personal/navigation/build/recent-stops;
}
trap cleanup EXIT
```

## Documentation

Kanban board: [Navigate Task Tracker](https://trello.com/invite/b/6933157a10bc19df00c1c541/ATTI4e1b50a7d285798fb6ad819c88690ef60542044A/navigate)
