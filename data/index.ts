import fse from "fs-extra";
import { OfficialLevelDocument, getAllEntries } from "./database";
import { LogStatus, log } from "./util";

async function getAllRawBoards() {
    const rawLevels: OfficialLevelDocument[] = await getAllEntries("officialLevels");
    rawLevels.sort((levelA, levelB) => levelA.order - levelB.order);
    
    const rawBoards: string[] = [];
    rawLevels.forEach(rawLevel => {
        rawBoards.push(rawLevel.board);
    });

    return rawBoards;
} 

async function main() {
    let writeLines = "";
    const rawBoards = await getAllRawBoards();
    log(`Successfully received ${rawBoards.length} levels from server.`, LogStatus.GOOD);
    
    rawBoards.forEach(board => {
        writeLines += board + "\n";
    });
    // writeLines = writeLines.slice(writeLines.length - 1);
    
    const levelsFile = "levels.txt"; // TODO: Either read this from the .cpp source file or have a project level config file.
    log(`Deleting existing levels file (${levelsFile}).`);
    fse.removeSync(levelsFile);

    if (fse.existsSync(levelsFile)) {
        log(`Unable to delete ${levelsFile}, ensure file is not being edited elsewhere.`, LogStatus.ERROR);
        return 1;
    }
    
    log(`Writing new raw board data to ${levelsFile} synchronously.`);
    fse.writeFileSync(levelsFile, writeLines);
    
    if (!fse.existsSync(levelsFile)) {
        log(`Write to ${levelsFile} failed! Levels have not been saved.`, LogStatus.ERROR);
        return 1;
    }

    log(`Successfully wrote ${rawBoards.length} raw boards to levels file!`, LogStatus.GOOD);
    return 0;
}

log("\nFetching official levels from server...");
main();